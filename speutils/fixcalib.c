/*
 *  fixcalib.c - Time-stamp: <Thu Dec 01 07:42:01 JST 2022>
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

double wl_center(int xDimDet,double *coef)
{
  return(poly0((1+xDimDet)/2.,6,coef));
}

int checkcalib(char *fname,double *coef, double *SpecCenterWlNm, double *wlcen0,
	       int verbose)
{
  WINXHEADER_STRUCT header;
  double *wl;
  int i,err;
  double wlcen;
  int xDimDet;
  double wlcen2;
  double wlcen_ref,wlcen_orig;
  double SpecCenterWlNm_ref,SpecCenterWlNm_orig;

  /* printf("%s\n",fname); */
  if((err = read_spe_header(fname,&header))>0)
    {
      wl=(double *) malloc(sizeof(double)*header.xdim);
    }
  else
    {
      printf("cannot open %s. Exiting.\n",fname);
      return EXIT_FAILURE;
    }

  poly((int) header.xdim, wl, 6, header.polynom_coeff_x);
  for(i=0;i<6;i++)
    {
      *(coef+i)=*(header.polynom_coeff_x+i);
    }
  
  wlcen= *SpecCenterWlNm = header.SpecCenterWlNm;
  wlcen2 =*wlcen0 = wl_center(header.xDimDet,header.polynom_coeff_x);
  if(verbose==1) {
    printf("%s: SpecCenterWlNm=%lf, wlcen=%lf\n",fname,wlcen,wlcen2);
  }
  
  if(fabs(wlcen-wlcen2)>1)
    return TRUE;
  else
    return FALSE;
}

int write_calibdata(char *fname,int xDimDet,double *coef)
{
  FILE *fp;
  int i;
  fp=fopen(fname,"w");

  fprintf(fp,"%d\n",xDimDet);
  for(i=0;i<6;i++)
    {
      fprintf(fp,"%lf\n",*(coef+i));
    }
  fclose(fp);
  return 1;
}

int read_calibdata(char *fname,int *xDimDet,double *coef)
{
  FILE *fp;
  int i;
  fp=fopen(fname,"r");
  
  fscanf(fp,"%d",xDimDet);
  for(i=0;i<6;i++)
    {
      fscanf(fp,"%lf",coef+i);
    }
  fclose(fp);
  return 1;
}

void usage(FILE *f)
{
  fprintf(f, "Usage: fixcalib [options] [<filename>]\n"
		  "Options:\n"
		  "         -h : this help message\n"
		  "         -v : verbose\n"
		  "         -c : check only\n"
		  "   -r <ref> : reference file (required)\n"
		  "   -o <out> : output file (required)\n"
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
  int with_reference=0,with_output=0;

  while ((c = getopt(argc, argv, "hvcr:o:O")) != -1)
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
	  //	  file_ref=optarg; // reference file (SPE or calib)
	  strcpy(file_ref,optarg);
	  if(strlen(file_ref)<=0){
	    printf("Invalid reference file\n");
	    return EXIT_FAILURE;
	  }
	  with_reference=1;
	  break;
	case 'o':
	  //	  file_dest=optarg; // destination (SPE or calib)
	  strcpy(file_dest,optarg);
	  if(strlen(file_dest)<=0){
	    printf("Invalid output file\n");
	    return EXIT_FAILURE;
	  }
	  with_output=1;
	  break; 
	/* case 'w': */
	/*   write_calib=1; // write to calib file */
	/*   break; */
	/* case 'f':  */
	/*   calibdata_ref=1; // read calibration data from file (not SPE) */
	/*   break; */
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
    read_calibdata(file_ref,&xDimDet,coef);
    wlcen_ref=wl_center(xDimDet,coef);
    calibration_ref_correct=TRUE;
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
  if(overwrite==1 && backup==1)
    {
      strcpy(file_back,file_orig);
      strcat(file_back,".bak");
      
      if((fp_dest=fopen(file_back,"wb"))==NULL)
	{
	  printf("cannot open file :%s\n",file_back);
	  return(-1);
	}
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
      printf("Backup %s created.\n",file_back);
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
  return 0;
}

