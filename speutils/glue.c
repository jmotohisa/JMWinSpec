/*
 *  glue.c - Time-stamp: <Tue Nov 29 10:03:58 JST 2022>
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

#include "WinSpecHeader25.h"
#include "readspe.h"

#define TRUE 1
#define FALSE 0

/*!
  @brief Calcuatate Calibrated wavlength from 
  @param[in] x: point number
  @return calibraated wavelength value
*/

void convert0(double *coef, int n_coef, double *spectrum_orig, int xdim,
	      double start, double end, double resolution,
	      double *wl_dest,int n_dest,
	      double **spectrum_dest,int **flg)
{
  int i,j;
  double wl;
  double *w, *z;
  double zLj,zRj,wLi,wRi,p;
  int *pixel_start,*pixel_end,pix_start,pix_end;
  
  // original data (w, spectrum_orig)
  w=(double *) malloc(sizeof(double)*(xdim+1)); //  *(w+i-1/2)
  for(i=0;i<=xdim;i++)
    {
      *(w+i)=  poly0(i+0.5, 6, coef); // NOTE:
    }

  // output data (z,spectrum_dest)
  z=(double *) malloc(sizeof(double)*(n_dest+1));  // *(z+j-1/2)
  *flg = (int *) malloc(sizeof(int)*n_dest);
  pixel_start = (int *) malloc(sizeof(int)*n_dest);
  pixel_end = (int *) malloc(sizeof(int)*n_dest);
  *spectrum_dest = (double *) malloc(sizeof(double)*n_dest);
  
  for(j=0;j<=n_dest;j++)
    {
      *(z+j)=start+resolution*(j-0.5);
    }

  // find starting and ending pixel in which conversion is enable
  // (both pixel_start and pixel_end should be >=0)
  for(j=0;j<n_dest;j++)
    {
      zLj=*(z+j);
      zRj=*(z+j+1);
      *(pixel_start+j)=-1;
      *(pixel_end+j)=-1;
      for(i=0;i<xdim;i++)
	{
	  wLi=*(w+i);
	  wRi=*(w+i+1);
	  if(zLj>wLi && zLj<wRi)
	    *(pixel_start+j)=i;
	  if(zRj>wLi && zRj<wRi)
	    *(pixel_end+j)=i;
	}
    }

  for(j=0;j<n_dest;j++)
    {
      pix_start=*(pixel_start+j);
      pix_end  =*(pixel_end+j);
      if(*(pixel_start+j)>=0 && *(pixel_end+j)>=0)
	{
	  *(*flg+j)=TRUE;
	  if(pix_start==pix_end) {
		i=pix_start;
	    p=*(spectrum_orig+i)/(*(w+i+1)-*(w+i))*(*(z+j+1)-*(z+j));
	  } else {
		i=pix_start;
	    p=*(spectrum_orig+i)/(*(w+i+1)-*(w+i))*(*(w+i+1)-*(z+j));
	    for(i=*(pixel_start+j)+1;i<*(pixel_end+j);i++)
	      {
		p+=*(spectrum_orig+i);
	      }
	    p+=*(spectrum_orig+i)/(*(w+i+1)-*(w+i))*(*(z+j+1)-*(w+i));
	  }
	}
      else
	{

	  *(*flg+j)=FALSE;
	  p=0;
	}
      *(*spectrum_dest+j)=p;
    }
  free(z);
  free(w);
  free(pixel_start);
  free(pixel_end);
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
  
  *n_dest=(end-start)/resolution+1;
  *wl_dest=(double *) malloc(sizeof(double)*(*n_dest));
  
  for(j=0;j<*n_dest;j++) {
    *(*wl_dest+j)=start+resolution*j;
  }
  convert0(coef, n_coef, spectrum_orig, xdim,
	   start, end, resolution,
	   *wl_dest,*n_dest,spectrum_dest,flg);
  return;
  
}

void getspe(char *fname, double **coef, double **wl, double **spectrum,
	    int *nxdim,int *nydim,int *numFrames)
{
  WINXHEADER_STRUCT header;
  double *data,*x;
  int err;
  int i;
  long n;
  double exp_sec;
  
  if((err = read_spe_header(fname,&header))>0)
    {
      data = (double *) malloc(sizeof(double)*header.xdim*header.ydim*header.NumFrames);
      x=(double *) malloc(sizeof(double)*header.xdim);
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
  exp_sec=header.exp_sec;

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
      *(*spectrum+i)=*(data+i)/exp_sec;
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

