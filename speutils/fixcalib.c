/*
 *  fixcalib.c - Time-stamp: <Tue Nov 29 23:17:35 JST 2022>
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

#include "WinSpecHeader25.h"
#include "readspe.h"

#define TRUE 1
#define FALSE 0

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
  fprintf(f, "Usage: fixcalib [options] [<filenames>]\n"
		  "Options:\n"
		  "         -h : this help message\n"
		  "         -V : print version number and copyright\n"
		  "         -w : just warn difference\n"
		  );
}

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

int main(int argc, char **argv)
{
  WINXHEADER_STRUCT header;
  char *file_orig;
  char *file_ref;
  char *file_dest;
  char *calibfile;
  double *data0;
  double *coef;
  double SpecCenterWlNm_orig,SpecCenterWlNm_ref;
  double wlcen_orig,wlcen_ref;
  int xDimDet;
  int i,n;
  FILE *fp_dest,*fp_orig;
  
  coef=(double *) malloc(sizeof(double)*6);
  
  if(checkcalib(file_orig,coef,&SpecCenterWlNm_orig,&wlcen_orig)==FALSE) {
    printf("Calibration is correct.\n");
    return 0;
  }

  // get reference calibration
  // from SPE file: 
  if(checkcalib(file_ref,coef,&SpecCenterWlNm_ref,&wlcen_ref)==TRUE)
    {
      printf("Calibration of the reference file is incorrect.\n");
      return 0;
    }
  // or from calibration data file
  read_calibdata(calibfile,&xDimDet,coef);
  wlcen_ref=wl_center(xDimDet,coef);

  // check reference calibration is appropriate
  if(fabs(SpecCenterWlNm_orig-wlcen_ref)>DBL_EPSILON)
    {
      printf("Calibration data of the reference file is inappropriate.\n");
      return 0;
    }

  //   double  polynom_coeff_x[6]      ;//      3263  polynom COEFFICIENTS            

  // read data and replace coefficients
  read_spe_header(file_orig, &header);
  n=header.xdim*header.ydim*header.NumFrames;
  for(i=0;i<6;i++) {
    *(header.polynom_coeff_x+i)=*(coef+i);
  }
  fp_orig=fopen(file_dest,"rb");
  fread(data0,sizeof(float),n,fp_orig);
  fclose(fp_orig);

  // write to file
  fp_dest=fopen(file_dest,"wb");
  fwrite(header, sizeof(WINXHEADER_STRUCT), 1, fp_dest);
  fwrite(data0,sizeof(float),n,fp_dest);
  fclose(fp_dest);

  free(coef);
  return 1;
}
