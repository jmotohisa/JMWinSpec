/*
 *  pyspeconvert.h - last saved: Time-stamp: <Thu Jul 10 14:38:56 JST 2025>
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
 *  $Id: pyspeconvert.h 2022-11-29 12:08:43 jmotohisa $
 */

/*! 
  @file pyspeconvert.h 
  @brief 
  @author J. Motohisa
*/

#ifndef _PYSPECONVERT_H
#define _PYSPECONVERT_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef	GLOBAL_VALUE_DEFINE
#define	GLOBAL
#else
#define	GLOBAL extern
#endif

GLOBAL void pyspeconvert(double *wl_dest, int n_dest,
			 double *spectrum_dest,int n_spectrum_dest,
			 int *flg, int n_flg,
			 char *fname, double start,double end,double resolution,
			 int dont_normalize_exp_sec,
			 int *xdim,int *ydim,int *NumFrames,
			 int *flag_wlcen,int *ierror);

GLOBAL int pyspeconvert0(double *spectrum, int n_spectrum,
			 double *coef, int n_coef,
			 double *wl_dest, int n_dest,
			 double *spectrum_dest, int n_spectrum_dest,
			 int *flg, int n_flg,
			 double start,double end,double resolution);

GLOBAL  int pycsvconvert0(double *wl, double *spectrum, int n_spectrum,
			  double *coef, int n_coef,
			  double *wl_dest, int n_dest,
			  double *spectrum_dest, int n_spectrum_dest,
			  int *flg, int n_flg,
			  double start,double end,double resolution);

#undef GLOBAL_VALUE_DEFINE
#undef GLOBAL

#ifdef __cplusplus
}
#endif

#endif  // _PYSPECONVERT_H
