/*
 *  pyglue2.c - Time-stamp: <Fri Dec 02 16:21:08 JST 2022>
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
 *  $Id: pyglue2.c 2022-12-01 21:59:08 jmotohisa $
 */

/*! 
  @file pyglue2.c 
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
#include "pyglue2.h"

#define CHECK(cond, msg) { if (!(cond)) { fprintf(stderr, "glue2 error: %s\n", msg); exit(EXIT_FAILURE); } }

#define CHK_MALLOC(p, t, n) {                                         \
	size_t CHK_MALLOC_n_tmp = (n);									  \
	(p) = (t *) malloc(sizeof(t) * CHK_MALLOC_n_tmp);				  \
	CHECK((p) || CHK_MALLOC_n_tmp == 0, "out of memory!");			  \
  }

/*!
  @brief
  @param[in]
  @param[out]
  @param[in,out]
  @return
*/

/* void pyglue2(double *wl_dest, int n_dest, */
/* 	     double *spectrum_dest,int n_spectrum_dest, */
/* 	     int *flg, int n_flg, */
/* 	     char *fname1, char *fname2, */
/* 	     double start,double end,double resolution, */
/* 	     int dont_normalize_exp_sec, */
/* 	     int *xdim,int *ydim,int *NumFrames, */
/* 	     int *flag_wlcen,int *ierror) */
/* { */
/*   WINXHEADER_STRUCT header; */
/*   int n_coef=6; */
/*   int i,j,n,err; */
/*   double *wl,*spectrum1,*spectrum2; */
/*   int *flg1,*flg2,*flg_dest; */

/*   n_dest=(end-start)/resolution+1; */

/*   CHK_MALLOC(wl_dest,double,n_dest); */
/*   CHK_MALLOC(spectrum1,double,n_dest); */
/*   CHK_MALLOC(flg1,int,n_dest); */
/*   CHK_MALLOC(spectrum2,double,n_dest); */
/*   CHK_MALLOC(flg2,int,n_dest); */

/*   for(j=0;j<n_dest;j++) { */
/*     *(wl_dest+j)=start+resolution*j; */
/*   } */
/*   getconveted(wl_dest, n_dest, */
/* 	      spectrum1,n_dest, */
/* 	      flg1, n_dest, */
/* 	      fname1, start,end,resolution, */
/* 	      dont_normalize_exp_sec, */
/* 	      &xdim, &ydim, &NumFrames, */
/* 	      &flag_wlcen,&ierror); */
/*   if(ierror!=0) */
/*     { */
/*       free(wl_dest); */
/*       free(spectrum1); */
/*       free(flg1); */
/*       free(spectrum2); */
/*       free(flg2); */
/*       return; */
/*     } */
/*   getconveted(wl_dest, n_dest, */
/* 	      spectrum2,n_dest, */
/* 	      flg2, n_dest, */
/* 	      fname2, start,end,resolution, */
/* 	      dont_normalize_exp_sec, */
/* 	      &xdim, &ydim, &NumFrames, */
/* 	      &flag_wlcen,&ierror); */
/*   if(ierror!=0) */
/*     { */
/*       free(wl_dest); */
/*       free(spectrum1); */
/*       free(flg1); */
/*       free(spectrum2); */
/*       free(flg2); */
/*       return; */
/*     } */
  
/*   CHK_MALLOC(spectrum_dest,double,n_dest); */
/*   CHK_MALLOC(flg_dest,int,n_dest); */
/*   glue2(n_dest, wl_dest, spectrum1, flg1, spectrum2, flg2, */
/* 	spectrum_dest, flg_dest); */
/*   dump_spectrum2(n_dest,flg_dest,wl_dest,spectrum_dest); */
/*   return; */
/* } */

/* !
   @brief Glue two spectra (for python binding)
   @param[in]
   @param[out]
   @param[in,out]
   @return

 */

void pyspeglue2(double *wl_dest, int n_dest,
		double *spectrum1_dest, int n_spectrum1_dest,
		int *flg1_dest, int n_flg1_dest,
		double *spectrum2_dest, int n_spectrum2_dest,
		int *flg2_dest, int n_flg2_dest,
		double *spectrum_2glued,int n_spectrum_2glued,
		int *flg_2glued, int n_flg_2glued)
{
  glue2(n_dest, wl_dest, spectrum1_dest, flg1_dest, spectrum2_dest, flg2_dest,
	spectrum_2glued, flg_2glued);
  return;
}


  
  

