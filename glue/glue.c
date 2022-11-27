/*
 *  glue.c - Time-stamp: <Sat Nov 26 19:16:36 JST 2022>
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
 *  $Id: glue.c 2022-11-26 14:18:36 jmotohisa $
 */

/*! 
  @file glue.c 
  @brief 
  @author J. Motohisa
  @date
*/

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <tgmath.h>

#define GLOBAL_VALUE_DEFINE
#include "glue.h"

#include "../ViewSPE/config.h"
#include "../ViewSPE/WinSpecHeader25.h"
#include "../ViewSPE/readspe.h"

#ifdef HAS_PLPLOT
#define MYDOUBLE PLFLT
#else
#define MYDOUBLE double
#endif

#define TRUE 1
#define FALSE 0

/*!
  @brief Calcuatate Calibrated wavlength from 
  @param[in] x: point number
  @return calibraated wavelength value
*/

double poly2(double x, int norder, double *coef)
{
  int j;
  double y;
  y=*(coef+norder-1);
  for(j=norder-2;j>=0;j--)
    {
      y=*(coef+j)+x*y;
    }
  return(y);
}

void convert0(double *coef, int n_coef, double *data, int xdim,
	      double *wl_dest,int n_dest,
	      double **ydata,int **flg)

{
  int i,j,n;
  double wl;
  double *wl_left,*wl_right;
  // original data
  wl_left=(double *) malloc(sizeof(double)*xdim);
  wl_right=(double *) malloc(sizeof(double)*xdim);
  for(i=0;i<xdim;i++)
    {
      *(wl_left+i)=  poly2(i-0.5, 6, coef);
      *(wl_right+i)= poly2(i+0.5, 6, coef);
    }

  // output data
  *ydata=(double *) malloc(sizeof(double)*n);
  *flg = (int *) malloc(sizeof(int)*n);
  
  for(j=0;j<n;j++)
    {
      //      wl=start+resolution*j;
      wl=*(wl_dest+j);      
      for(i=0;i<xdim;i++)
	{
	  if(wl>*(wl_left+i) && wl<*(wl_right+i))
	    {
	      *(*ydata+j)=(wl-(*(wl_left+i)))/(*(wl_right+i)-(*(wl_right+i)))*(*(data+i));
	      *(*flg+j)=TRUE;
	    }
	  else
	    {
	      *(*ydata+j)=0;
	      *(*flg+j)=FALSE;
	    }
	}
    }
  return;
}


/*!
  @brief 
  @param[in] *coef: coefficientes for calibration
  @param[in] n_coef: order of polynomial for the wavelength calibration
  @param[in] *spectrum_orig: original intensity data
  @param[in] xdim: size of the intensity data

  @param[out] wl_dest: 
  @param[out] ydata: 
  @param[in,out]
  @return
*/
void convert(double *coef, int n_coef, double *spectrum_orig, int xdim,
	     double start, double end, double resolution,
	     double **spectrum_dest,int **flg, double **wl_dest, int *n_dest)
{
  int j,n;
  
  *n_dest=(start-end)/resolution+1;
  *wl_dest=(double *) malloc(sizeof(double)*(*n_dest));

  
  for(j=0;j<*n_dest;j++) {
    *(*wl_dest+j)=start+resolution*j;
  }
  convert0(coef, n_coef, spectrum_orig, xdim,
	   *wl_dest,*n_dest,spectrum_dest,flg);
  return;
  
}

void getspe(char *fname, double **coef, double **wl, double **spectrum,
	    int *nxdim,int *nydim,int *numFrames)
{
  WINXHEADER_STRUCT header;
  MYDOUBLE *data,*x;
  int err;
  int i;
  long n;
  
  if((err = read_spe_header(fname,&header))>0)
    {
      data = (MYDOUBLE *) malloc(sizeof(MYDOUBLE)*header.xdim*header.ydim*header.NumFrames);
      x=(MYDOUBLE *) malloc(sizeof(MYDOUBLE)*header.xdim);
    } else {
    
  }
  err = read_spe_data(fname,data,header);
  poly((int) header.xdim, x,6, header.polynom_coeff_x);
  
  n=header.xdim*header.ydim*header.NumFrames;
  *wl = (double *) malloc(sizeof(double)*header.xdim);
  *spectrum = (double *) malloc(sizeof(double)*n);
  *coef =  (double *) malloc(sizeof(double)*6);
  *nxdim=header.xdim;
  *nydim=header.ydim;
  *numFrames=header.NumFrames;

  /* copy data */
  for(i=0;i<6;i++) {
    *(*coef+i)=*(header.polynom_coeff_x+i);
  }
  for(i=0;i<header.xdim;i++)
    {
      *(*wl+i)=*(x+i);
    }
  for(i=0;i<n;i++)
    {
      *(*spectrum+i)=*(data+i);
    }
  
  free(data);
  free(x);
  return;
}

void getconvertedspe(char *fname,
		     double start, double end, double resolution,
		     double **spectrum_dest,int **flg, double **wl_dest, int *n_dest)
{
  double *spectrum,*coef,*wl;
  int n_coef=6;
  int nxdim,nydim,numFrames;

  // assume nydim=1 and numFrames=1
  getspe(fname, &coef, &wl, &spectrum,
	 &nxdim,&nydim,&numFrames);
  convert(coef, n_coef, spectrum, nxdim,
	  start, end, resolution,
	  spectrum_dest,flg, wl_dest, n_dest);
  free(spectrum);
  free(coef);
  free(wl);
  return;
}


void glue(char *fname[],int nfile, double start, double end, double resolution,
	  double **y_dest, double **wl_dest, int *n_dest)
{
  *n_dest=(start-end)/resolution+1;
  *wl_dest=(double *) malloc(sizeof(double)*(*n_dest));
  *y_dest=(double *) malloc(sizeof(double)*(*n_dest));

}

