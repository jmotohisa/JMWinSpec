/*
 *  fixcalib.c - Time-stamp: <Wed Nov 30 12:41:40 JST 2022>
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

int checkcalib(char *fname,double *coef, double *SpecCenterWlNm, double *wlcen0)
{
  WINXHEADER_STRUCT header;
  double *wl;
  int i,err;
  double wlcen;
  int xDimDet;
  double wlcen2;
  double wlcen_ref,wlcen_orig;
  double SpecCenterWlNm_ref,SpecCenterWlNm_orig;

  if((err = read_spe_header(fname,&header))>0)
    {
      wl=(double *) malloc(sizeof(double)*header.xdim);
    } else {
  }
  
  poly((int) header.xdim, wl, 6, header.polynom_coeff_x);
  for(i=0;i<6;i++)
    {
      *(coef+i)=*(header.polynom_coeff_x+i);
    }
  
  wlcen= *SpecCenterWlNm = header.SpecCenterWlNm;
  wlcen2 =*wlcen0 = wl_center(header.xDimDet,header.polynom_coeff_x);

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
		  );
}

int main(int argc, char **argv)
{
  WINXHEADER_STRUCT header;
  char file_orig[MAXLEN];
  char file_ref[MAXLEN];
  char file_dest[MAXLEN];
  float *data_float;
  int *data_int;
  short *data_short;
  unsigned  short *data_ushort;
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
  int calibration_orig_correct,calibration_ref_correct;

  while ((c = getopt(argc, argv, "hvcr:o:")) != -1)
	switch (c) {
	case 'h':
	  usage(stdout);
	  return EXIT_SUCCESS;
	case 'v':
	  verbose=1;
	  return EXIT_SUCCESS;
	case 'c':
	  checkonly = 1; // check only (both orig and reference SPE file)
	  break;
	case 'r':
	  //	  file_ref=optarg; // reference file (SPE or calib)
	  strcpy(file_ref,optarg);
	  if(strlen(file_ref)<=0){
	    printf("invalid reference\n");
	    return 0;
	  }
	  break;
	case 'o':
	  //	  file_dest=optarg; // destination (SPE or calib)
	  strcpy(file_dest,optarg);
	  if(strlen(file_dest)<=0){
	    printf("invalid destination\n");
	    return 0;
	  }
	  break; 
	/* case 'w': */
	/*   write_calib=1; // write to calib file */
	/*   break; */
	/* case 'f':  */
	/*   calibdata_ref=1; // read calibration data from file (not SPE) */
	/*   break; */
	default:
	  fprintf(stderr, "Invalid argument -%c\n", c);
	  usage(stderr);
	  return EXIT_FAILURE;
	}
  
  if (optind == argc) {  /* no parameters left */
	usage(stderr);
	return EXIT_FAILURE;
  }
  strcpy(file_orig,argv[optind]);
  coef=(double *) malloc(sizeof(double)*6);
  if(checkcalib(file_orig,coef,&SpecCenterWlNm_orig,&wlcen_orig)==FALSE) {
    calibration_orig_correct = TRUE;
    printf("ref: SpecCenterWlNm=%lf, wlcen=%lf\n",SpecCenterWlNm_orig,wlcen_orig);
    printf("Calibration in file %s is correct.\n",file_orig);
  } else {
    printf("ref: SpecCenterWlNm=%lf, wlcen=%lf\n",SpecCenterWlNm_orig,wlcen_orig);
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
    if(checkcalib(file_ref,coef,&SpecCenterWlNm_ref,&wlcen_ref)==TRUE)
    {
      calibration_ref_correct=FALSE;
      printf("ref: SpecCenterWlNm=%lf, wlcen=%lf\n",SpecCenterWlNm_ref,wlcen_ref);
      printf("Calibration of the reference file %s is incorrect.\n",file_ref);
    } else {
      printf("ref: SpecCenterWlNm=%lf, wlcen=%lf\n",SpecCenterWlNm_ref,wlcen_ref);
      printf("Calibration of the reference file %s is correct.\n",file_ref);
        calibration_ref_correct=TRUE;
    }
  }

  // check reference calibration is appropriate
  if(fabs(SpecCenterWlNm_orig-wlcen_ref)>1)
    {
      printf("Calibration data of the reference file is inappropriate.\n");
      return 0;
    }
  
  if(checkonly==1 || calibration_orig_correct==TRUE || calibration_ref_correct==FALSE)
    {
      free(coef);
      return 0;
    }

  //   double  polynom_coeff_x[6]      ;//      3263  polynom COEFFICIENTS            

  // read data
  read_spe_header(file_orig, &header);
  datatype=header.datatype;
  n=header.xdim*header.ydim*header.NumFrames;
  
  fp_orig=fopen(file_orig,"rb");
  fseek(fp_orig,sizeof(WINXHEADER_STRUCT),SEEK_SET);
  switch(datatype) {
  case 0:
    data_float=(float *)malloc(sizeof(float)*n);
    fread(data_float,sizeof(float),n,fp_orig);
    break;
  case 1:
    data_int=(int *)malloc(sizeof(int)*n);
    fread(data_int,sizeof(int),n,fp_orig);
    break;
  case 2:
    data_short=(short *)malloc(sizeof(short)*n);
    fread(data_short,sizeof(short),n,fp_orig);
    break;
  case 3:
    data_short=(unsigned short *)malloc(sizeof(unsigned short)*n);
    fread(data_ushort,sizeof(unsigned short),n,fp_orig);
    break;
  default:
    printf("Data type error. Exiting\n");
    return 0;
    break;
  }
  fclose(fp_orig);

  // replace coefficients
  for(i=0;i<6;i++) {
    *(header.polynom_coeff_x+i)=*(coef+i);
  }

  // write to file
  fp_dest=fopen(file_dest,"wb");
  fwrite(&header, sizeof(WINXHEADER_STRUCT), 1, fp_dest);
  switch(datatype) {
  case 0:
    fwrite(data_float,sizeof(float),n,fp_dest);
    break;
  case 1:
    fwrite(data_int,sizeof(int),n,fp_dest);
    break;
  case 2:
    fwrite(data_short,sizeof(short),n,fp_dest);
    break;
  case 3:
    fwrite(data_ushort,sizeof(unsigned short),n,fp_dest);
    break;
  default:
    printf("Data type error. Exiting\n");
    return 0;
    break;
  }
  fclose(fp_dest);

  switch(datatype) {
  case 0:
    free(data_float);
    break;
  case 1:
    free(data_int);
    break;
  case 2:
    free(data_short);
    break;
  case 3:
    free(data_ushort);
    break;
  default:
    printf("Data type error. Exiting\n");
    return 0;
    break;
  }
  printf("Calibration data of %s was successfully fixed using %s and written to %s",
		 file_orig,file_ref,file_dest);
  free(coef);
  return 0;
}

