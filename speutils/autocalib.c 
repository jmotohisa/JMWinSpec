/*
 *  autocalib.c - Time-stamp: <Wed Jul 12 15:50:52 JST 2023>
 *
 *   Copyright (c) 2023  jmotohisa (Junichi Motohisa)  <motohisa@ist.hokudai.ac.jp>
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
 *  $Id: autocalib.c 2023-07-12 14:47:50 jmotohisa $
 */

/*! 
  @file autocalib.c 
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

#include "config.h"
#include "copyright.h"

#include "WinSpecHeader25.h"
#include "readspe.h"
#include "calib_util.h"

#define TRUE 1
#define FALSE 0
#define MAXLEN 256

/* #define GLOBAL_VALUE_DEFINE */
/* #include "autocalib.h" */

/*!
  @brief
  @param[in]
  @param[out]
  @param[in,out]
  @return
*/

void usage(FILE *f)
{
  fprintf(f, "Usage: autocalib [Options] <filenames>\n"
	     "Options:\n"
	     "         -h : this help message\n"
	     "         -V : print version number and copyright\n"
	  /* "         -w : just warn difference\n" */
	  /* "         -c : dump coeffients\n" */
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
  int warn = 0;
  int dump_coefs=0;
  int err;
  int ifile;
  double *x;
  int calibration_orig_correct,calibration_ref_correct;
  int backup;
  int overwrite=1;
  
  while ((c = getopt(argc, argv, "hV")) != -1)
    switch (c) {
    case 'h':
      usage(stdout);
      return EXIT_SUCCESS;
    case 'V':
      printf("checkWL " PACKAGE_VERSION " by Junichi Motohisa\n"
	     COPYRIGHT);
      return EXIT_SUCCESS;
      /* case 'w': */
      /*   warn = 1; */
      /*   break; */
      /* case 'c': */
      /*   dump_coefs = 1; */
      /*   break; */
      /* default: */
      /*   fprintf(stderr, "Invalid argument -%c\n", c); */
      /*   usage(stderr); */
      /*   return EXIT_FAILURE; */
    }
  backup=1;
  
  if (optind == argc) {  /* no parameters left */
    usage(stderr);
    return EXIT_FAILURE;
  }
  
  coef=(double *) malloc(sizeof(double)*6);
  
  for (ifile = optind; ifile < argc; ++ifile) {
    /* read data from SPE file*/
    strcpy(file_orig,argv[ifile]);
    coef=(double *) malloc(sizeof(double)*6);
    if(checkcalib(file_orig,coef,&SpecCenterWlNm_orig,&wlcen_orig,verbose)==FALSE) {
      calibration_orig_correct = TRUE;
    } else {
      printf("Calibration in file %s is incorrect. Actual wlcen is %lf.\n",file_orig,wlcen_orig);
      calibration_orig_correct = FALSE;
      
      // read calibration data ( xxxx.cal, where xxxx represents wavelength)
      snprintf(file_ref,MAXLEN,"%d.cal",(int) wlcen_orig);
      printf("Reading calibration from file %s.\n",file_ref);
      read_calibdata(file_ref,&SpecCenterWlNm_ref,&xDimDet,coef);
      wlcen_ref=wl_center(xDimDet,coef);
      if((fabs(SpecCenterWlNm_ref-wlcen_ref))<1 && fabs(wlcen_orig-wlcen_ref))
	{
	  printf("Calibration of the reference file %s is correct.\n",file_ref);
	  calibration_ref_correct=TRUE;
	}
      else
	{
	  printf("Calibration of the reference file %s is incorrect.\n",file_ref);
	  calibration_ref_correct=FALSE;
	  return EXIT_FAILURE;
	}	 
      
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
    }
    free(coef);
  }
  return EXIT_SUCCESS;
}
