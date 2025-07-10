/*
 *  pyspeconvert.c - Time-stamp: <Thu Jul 10 15:01:44 JST 2025>
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
 *  $Id: pyspeconvert.c 2022-11-29 11:49:25 jmotohisa $
 */

/*! 
  @file pyspeconvert.c 
  @brief 
  @author J. Motohisa
  @date
*/

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <tgmath.h>

#include "../speutils/WinSpecHeader25.h"
#include "../speutils/readspe.h"
#include "../speutils/glue.h"
#include "../speutils/convertspe.h"

#define GLOBAL_VALUE_DEFINE
#include "pyspeconvert.h"

/*!
  @brief
  @param[in]
  @param[out]
  @param[in,out]
  @return
*/

void pyspeconvert(double *wl_dest, int n_dest,
		  double *spectrum_dest,int n_spectrum_dest,
		  int *flg, int n_flg,
		  char *fname, double start,double end,double resolution,
		  int dont_normalize_exp_sec,
  		  int *xdim,int *ydim,int *NumFrames,
		  int *flag_wlcen,int *ierror)
{
    WINXHEADER_STRUCT header;
    int n_coef=6;
    int i,n,err;
    double *wl,*spectrum;
    /* printf("%s\n",fname); */
    /* printf("%lf\n",start); */
    /* printf("%lf\n",end); */
    /* printf("%lf\n",resolution); */
    /* printf("%d\n",dont_normalize_exp_sec); */

    /* read data from SPE file*/
    if((err = read_spe_header(fname,&header))>0)
      {
	*xdim=header.xdim;
	*ydim=header.ydim;
	*NumFrames = header.NumFrames;
	n=header.xdim*header.ydim*header.NumFrames;
	spectrum = (double *) malloc(sizeof(double)*n);
	wl=(double *) malloc(sizeof(double)*header.xdim);
      } else {
      *ierror = 1;
      return;
    }
    
    err = read_spe_data(fname,spectrum,header);
    poly((int) header.xdim, wl, n_coef, header.polynom_coeff_x);

    // Check if libration data and Center of the Spectrometer match
    double wlcen=header.SpecCenterWlNm;
    int xDimDet=header.xDimDet;
    double wlcen2 = poly0((1+xDimDet)/2.,6,header.polynom_coeff_x);
    if(fabs(wlcen-wlcen2)>1)
      {
	*flag_wlcen=1;
	*ierror=2;
      }
    else
      *flag_wlcen=0;

    if(dont_normalize_exp_sec!=1)
      {
	for(i=0;i<n;i++)
	  *(spectrum+i)=*(spectrum+i)/header.exp_sec;
      }
    
    if(start<=0)
      start=*(wl);
    if(end<=0)
      end=*(wl+header.xdim-1);
    if(resolution<=0)
      resolution=1;

    convert0(header.polynom_coeff_x, n_coef, spectrum, header.xdim,
	     start, end, resolution,
	     wl_dest, n_dest,spectrum_dest,flg);
    //    dump_spectrum("",n_dest,wl_dest,spectrum_dest);
    /* dump_spectrum2(n_dest, flg, wl_dest, spectrum_dest); */
    
    *ierror=0;
    free(wl);
    free(spectrum);

    return;
}

int pyspeconvert0(double *spectrum, int n_spectrum,
		  double *coef, int n_coef,
		  double *wl_dest, int n_dest,
		  double *spectrum_dest, int n_spectrum_dest,
		  int *flg, int n_flg,
		  double start,double end,double resolution)
{
  convert0(coef, n_coef, spectrum, n_spectrum,
	     start, end, resolution,
	     wl_dest, n_dest, spectrum_dest,flg);
  return 0;
}

void pycsvconvert(double *wl_orig,double *spectrum_orig,int n_orig,
		  double *wl_dest, int n_dest,
		  double *spectrum_dest,int n_spectrum_dest,
		  int *flg, int n_flg,
		  char *fname, double start,double end,double resolution,
		  int dont_normalize_exp_sec,
  		  int *xdim,int *ydim,int *NumFrames,
		  int *flag_wlcen,int *ierror)
{
    int i,n,err;

    if(start<=0)
      start=*(wl_orig);
    if(end<=0)
      end=*(wl_orig+n_orig-1);
    if(resolution<=0)
      resolution=1;

    convert00(wl_orig,spectrum_orig, n_orig,
	      start, end, resolution,
	      wl_dest, n_dest,spectrum_dest,flg);
    //    dump_spectrum("",n_dest,wl_dest,spectrum_dest);
    /* dump_spectrum2(n_dest, flg, wl_dest, spectrum_dest); */
    
    *ierror=0;

    return;
}

int pycsvconvert0(double *wl, double *spectrum, int n_spectrum,
		 double *coef, int n_coef,
		 double *wl_dest, int n_dest,
		 double *spectrum_dest, int n_spectrum_dest,
		 int *flg, int n_flg,
		 double start,double end,double resolution)
{
  convert00(wl, spectrum, n_spectrum,
	   start, end, resolution,
	   wl_dest, n_dest, spectrum_dest,flg);
  return 0;
}

