/*
 *  calib_util.c - Time-stamp: <Wed Jul 12 15:46:59 JST 2023>
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
 *  $Id: calib_util.c 2023-07-12 15:42:03 jmotohisa $
 */

/*! 
  @file calib_util.c 
  @brief 
  @author J. Motohisa
  @date
*/

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <tgmath.h>

#include "WinSpecHeader25.h"
#include "readspe.h"

#define GLOBAL_VALUE_DEFINE
#include "calib_util.h"

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
  //  int xDimDet;
  double wlcen2;
  //  double wlcen_ref,wlcen_orig;
  //  double SpecCenterWlNm_ref,SpecCenterWlNm_orig;

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

int write_calibdata(char *fname,double wlcen,int xDimDet,double *coef)
{
  FILE *fp;
  int i;
  fp=fopen(fname,"w");

  fprintf(fp,"%lf\n",wlcen);
  fprintf(fp,"%d\n",xDimDet);
  for(i=0;i<6;i++)
    {
      fprintf(fp,"%lf\n",*(coef+i));
    }
  fclose(fp);
  return 1;
}

int read_calibdata(char *fname,double *wlcen, int *xDimDet,double *coef)
{
  FILE *fp;
  int i;
  fp=fopen(fname,"r");

  fscanf(fp,"%lf",wlcen);
  fscanf(fp,"%d",xDimDet);
  for(i=0;i<6;i++)
    {
      fscanf(fp,"%lf",coef+i);
    }
  fclose(fp);
  return 1;
}

