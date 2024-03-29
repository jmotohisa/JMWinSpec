/*
 *  fixcalib.c - Time-stamp: <Wed Jul 12 15:44:27 JST 2023>
 *
 *   Copyright (c) 2022  jmotohisa (Junichi Motohisa)  <motohisa@ist.hokudai.ac.jp>
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  $Id: fixcalib.c 2022-11-29 21:36:17 jmotohisa $
 */

/*! 
  @file fixcalib.c 
  @brief 
  @author J. Motohisa
  @date
*/

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <tgmath.h>
#include <unistd.h>
#include <float.h>
#include <string.h>

#include "WinSpecHeader25.h"
#include "readspe.h"
#include "calib_util.h"

#define STR_ALLOC_COPY(dest,orig) \\
  dest = (char *) malloc(sizeof(char)*(strlen(orig)+1));\\
  strcpy(dest,orig);

#define TRUE 1
#define FALSE 0
#define MAXLEN 256

#define GLOBAL_VALUE_DEFINE
/* #include "fixcalib.h" */

/*!
  @brief
  @param[in]
  @param[out]
  @param[in,out]
  @return
*/

void usage(FILE *f)
{
  fprintf(f, "Usage: fixcalib [options] [<filename>]\n"
		  "Options:\n"
		  "         -h : this help message\n"
		  "         -v : verbose\n"
		  "         -c : check only\n"
		  "   -r <ref> : reference file (required)\n"
		  "   -o <out> : output file (required unless -O option)\n"
		  "   -w <out> : write calibration data to <out>\n"
		  "         -O : overwite input file\n"
		  "         -B : DO NOT create backup\n"
		  );
}

int main(int argc, char **argv)
{
  WINXHEADER_STRUCT header;
  char file_orig[MAXLEN];
  char file_ref[MAXLEN];
  char file_dest[MAXLEN];
  char file_back[MAXLEN];
  char file_calib[MAXLEN];
  void *data;
  int datatype;
  double *coef;
  double SpecCenterWlNm_orig,SpecCenterWlNm_ref;
  double wlcen_orig,wlcen_ref;
  int xDimDet;
  int i,n;
  FILE *fp_dest,*fp_orig;
  int c;
  int verbose=0;
  int checkonly = 0;
  int write_calib=0;
  int calibdata_ref=0;
  int overwrite=0;
  int backup=1;
  int calibration_orig_correct,calibration_ref_correct;
  int with_output=0;

  while ((c = getopt(argc, argv, "hvcr:o:w:O")) != -1)
    switch (c) {
    case 'h':
      usage(stdout);
      return EXIT_SUCCESS;
    case 'v':
      verbose=1;
      break;
    case 'c':
      checkonly = 1; // check only (both orig and reference SPE file)
      break;
    case 'r':
      strcpy(file_ref,optarg); // reference file (SPE or calib)
      if(strlen(file_ref)<=0){
	printf("Invalid reference file\n");
	return EXIT_FAILURE;
      }
      break;
    case 'o':
      strcpy(file_dest,optarg); // destination (SPE or calib)
      if(strlen(file_dest)<=0)
	{
	  printf("Invalid output file\n");
	  return EXIT_FAILURE;
	}
      with_output=1;
      break;
    case 'w':
      write_calib=1; // write to calib file
      strcpy(file_calib,optarg);
      if(strlen(file_calib)<=0){
	printf("Invalid output for calibration data\n");
	return EXIT_FAILURE;
      }
      break;
    case 'f':
      calibdata_ref=1; // read calibration data from file (not SPE)
      break;
    case 'O':
      overwrite=1;
      with_output=1;
      break;
    case 'B':
      backup=0;
      break;
    default:
      fprintf(stderr, "Invalid argument -%c\n", c);
      usage(stderr);
      return EXIT_FAILURE;
    }
  
  if (optind == argc) {  /* no parameters left */
	usage(stderr);
	return EXIT_FAILURE;
  }

  if(with_output==0)
    {
      printf("No output file specified. Exiting.\n");
      return EXIT_FAILURE;
    }
  
  strcpy(file_orig,argv[optind]);
  coef=(double *) malloc(sizeof(double)*6);
  if(checkcalib(file_orig,coef,&SpecCenterWlNm_orig,&wlcen_orig,verbose)==FALSE) {
    calibration_orig_correct = TRUE;
  } else {
    printf("Calibration in file %s is incorrect.\n",file_orig);
    calibration_orig_correct = FALSE;
  }

  // get reference calibration
  if(calibdata_ref==1) {
    // or from calibration data file
    printf("Reading calibration from file %s.\n",file_ref);
    read_calibdata(file_ref,&SpecCenterWlNm_ref,&xDimDet,coef);
    wlcen_ref=wl_center(xDimDet,coef);
    if((fabs(SpecCenterWlNm_ref-wlcen_ref))<1)
      {
	printf("Calibration of the reference file %s is correct.\n",file_ref);
	calibration_ref_correct=TRUE;
      }
    else
      {
	printf("Calibration of the reference file %s is incorrect.\n",file_ref);
	calibration_ref_correct=FALSE;
      }	 
  } else {
    // from SPE file:
    if(checkcalib(file_ref,coef,&SpecCenterWlNm_ref,&wlcen_ref,verbose)==TRUE)
    {
      calibration_ref_correct=FALSE;
      printf("Calibration of the reference file %s is incorrect.\n",file_ref);
    } else {
      printf("Calibration of the reference file %s is correct.\n",file_ref);
        calibration_ref_correct=TRUE;
    }
  }

  // check reference calibration is appropriate
  if(fabs(SpecCenterWlNm_orig-wlcen_ref)>1)
    {
      printf("Calibration data of the reference file is inappropriate. Exiting.\n");
      return EXIT_FAILURE;
    }
  
  if(checkonly==1 || calibration_orig_correct==TRUE || calibration_ref_correct==FALSE)
    {
      printf("Do nothing.\n");
      free(coef);
      return 0;
    }

  if(write_calib==1) {
    write_calibdata(file_calib,SpecCenterWlNm_ref,xDimDet,coef);
  }    
  // double  polynom_coeff_x[6]      ;//      3263  polynom COEFFICIENTS            

  // read data from original
  read_spe_header(file_orig, &header);
  datatype=header.datatype;
  n=header.xdim*header.ydim*header.NumFrames;
  
  printf("Opening original file:%s\n",file_orig);
  if((fp_orig=fopen(file_orig,"rb"))==NULL)
    {
      printf("cannot open file :%s\n",file_orig);
      return (-1);
    }
  fseek(fp_orig,sizeof(WINXHEADER_STRUCT),SEEK_SET);
  switch(datatype) {
  case 0:
    data=(void *)malloc(sizeof(float)*n);
    fread(data,sizeof(float),n,fp_orig);
    break;
  case 1:
    data =(void *)malloc(sizeof(int)*n);
    fread(data,sizeof(int),n,fp_orig);
    break;
  case 2:
    data=(void *)malloc(sizeof(short)*n);
    fread(data,sizeof(short),n,fp_orig);
    break;
  case 3:
    data=(void *)malloc(sizeof(unsigned short)*n);
    fread(data,sizeof(unsigned short),n,fp_orig);
    break;
  default:
    printf("Data type error. Exiting\n");
    return -1;
    break;
  }
  fclose(fp_orig);
  printf("file %s closed\n",file_orig);

  // create backup
  if(backup==1)
    {
      strcpy(file_back,file_orig);
      strcat(file_back,".bak");
      if(rename(file_orig,file_back)!=0)
	{
	  printf("Bckup file creation failed. Exiting\n");
	  return EXIT_FAILURE;
	}
      else
	{
	  printf("Backup %s created.\n",file_back);
	}
    }
  
  // replace coefficients
  for(i=0;i<6;i++) {
    *(header.polynom_coeff_x+i)=*(coef+i);
  }
  // write to file
  if(overwrite==1)
    {
      strcpy(file_dest,file_orig);
    }
  fp_dest=fopen(file_dest,"wb");
  fwrite(&header, sizeof(WINXHEADER_STRUCT), 1, fp_dest);
  switch(datatype) {
  case 0:
    fwrite(data,sizeof(float),n,fp_dest);
    break;
  case 1:
    fwrite(data,sizeof(int),n,fp_dest);
    break;
  case 2:
    fwrite(data,sizeof(short),n,fp_dest);
    break;
  case 3:
    fwrite(data,sizeof(unsigned short),n,fp_dest);
    break;
  default:
    printf("Data type error. Exiting\n");
    return -1;
    break;
  }
  
  fclose(fp_dest);
  free(data);
  printf("Calibration data of %s was successfully fixed using %s and written to %s\n",
		 file_orig,file_ref,file_dest);
  free(coef);
  return EXIT_SUCCESS;
}

