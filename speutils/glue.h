/*
 *  glue.h - last saved: Time-stamp: <Thu Jul 10 14:41:01 JST 2025>
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
 *  $Id: glue.h 2022-11-26 15:41:19 jmotohisa $
 */

/*! 
  @file glue.h 
  @brief 
  @author J. Motohisa
*/

#ifndef _GLUE_H
#define _GLUE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef	GLOBAL_VALUE_DEFINE
#define	GLOBAL
#else
#define	GLOBAL extern
#endif

GLOBAL void getspe(char *fname, double **coef, double **wl, double **spectrum,
		   int *nxdim,int *nydim,int *numFrames);
GLOBAL void convert(double *coef, int n_coef, double *spectrum_orig, int xdim,
		    double start, double end, double resolution,
		    double **spectrum_dest,int **flg, double **wl_dest, int *n_dest);
GLOBAL void convert_wl(double *wl_orig, double *spectrum_orig, int xdim,
		       double start, double end, double resolution,
		       double **spectrum_dest,int **flg, double **wl_dest, int *n_dest);
GLOBAL void convert0(double *coef, int n_coef, double *spectrum_orig, int xdim,
		     double start, double end, double resolution,
		     double *wl_dest,int n_dest,
		     double *spectrum_dest,int *flg);
GLOBAL void convert00(double *wl_orig, double *spectrum_orig, int xdim,
		      double start, double end, double resolution,
		      double *wl_dest,int n_dest,
		      double *spectrum_dest,int *flg);

GLOBAL void check_pix_start_end(int n, int ss, int *flg, int *start, int *end);
  
GLOBAL void glue_2spectra(int n, double *wl, double *spec1, int *flg1, double *spec2, int *flg2,
			  double *spec_dest, int *flg_dest, int edge_processing_mode);
  
#undef GLOBAL_VALUE_DEFINE
#undef GLOBAL

#ifdef __cplusplus
}
#endif

#endif  // _GLUE_H
