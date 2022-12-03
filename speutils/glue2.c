/*
 *  glue2.c - Time-stamp: <Fri Dec 02 20:17:10 JST 2022>
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
 *  $Id: glue2.c 2022-12-01 14:41:35 jmotohisa $
 */

/*! 
  @file glue2.c 
  @brief 
  @author J. Motohisa
  @date
*/

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <tgmath.h>
#include <unistd.h>

#include "WinSpecHeader25.h"
#include "readspe.h"

#define CHECK(cond, msg) { if (!(cond)) { fprintf(stderr, "glue2 error: %s\n", msg); exit(EXIT_FAILURE); } }

#define CHK_MALLOC(p, t, n) {                                         \
	size_t CHK_MALLOC_n_tmp = (n);									  \
	(p) = (t *) malloc(sizeof(t) * CHK_MALLOC_n_tmp);				  \
	CHECK((p) || CHK_MALLOC_n_tmp == 0, "out of memory!");			  \
  }

#include "glue.h"

#define GLOBAL_VALUE_DEFINE
#include "glue2.h"

/*!
  @brief
  @param[in]
  @param[out]
  @param[in,out]
  @return
*/

// glue 2 spectra

void usage(FILE *f)
{
  fprintf(f, "Usage: convertspe [options] <filename1> <filename2>\n"
		  "Options:\n"
		  "         -h : this help message\n"
		  "         -s  <val> : starting waveleng\n"
		  "         -e  <val> : ending waveleng\n"
		  "         -r  <val> : resolution\n"
		  "         -c : check wlcen\n"
		  "         -n : dont normalize with exp_sec\n"
		  
		  );
}

int getconveted(double *wl_dest, int n_dest,
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
      return 1;
    }
    
    err = read_spe_data(fname,spectrum,header);
    poly((int) header.xdim, wl, n_coef, header.polynom_coeff_x);

    // Check if calibration data and Center of the Spectrometer match
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
    
    *ierror=0;
    free(wl);
    free(spectrum);

    return 1;
}


int main(int argc, char **argv)
{
  WINXHEADER_STRUCT header;
  int c;
  int verbose = 0;
  int image = 0;
  int label = 0;
  int normalization = 0;
  int err;
  int ifile;
  long i;
  double start,end,resolution=1;
  double *spectrum_dest,*wl_dest;
  int istart=0,iend=0;
  int n_dest,*flg;
  int check_SpecCenterWlNm,dont_normalize_exp_sec;
  double *spectrum1,*spectrum2;
  int *flg1,*flg2,*flg_dest;
  int j;
  int xdim,ydim,NumFrames,flag_wlcen,ierror;

  while ((c = getopt(argc, argv, "hvs:e:r:cn")) != -1)
	switch (c) {
	case 'h':
	  usage(stdout);
	  return EXIT_SUCCESS;
	case 'v':
	  verbose=1;
	  break;
	case 's':
	  start=atof(optarg);
	  istart=1;
	  break;
	case 'e':
	  end=atof(optarg);
	  iend=1;
	  break;
	case 'r':
	  resolution=atof(optarg);
	  break;
	case 'c':
	  check_SpecCenterWlNm=TRUE;
	  break;
	case 'n':
	  dont_normalize_exp_sec=TRUE;
	  break;
	default:
	  fprintf(stderr, "Invalid argument -%c\n", c);
	  usage(stderr);
	  return EXIT_FAILURE;
	}
  
  if (optind == argc+1) {  /* no parameters left */
	usage(stderr);
	return EXIT_FAILURE;
  }

  ifile=optind;
  n_dest=(end-start)/resolution+1;

  CHK_MALLOC(wl_dest,double,n_dest);
  CHK_MALLOC(spectrum1,double,n_dest);
  CHK_MALLOC(flg1,int,n_dest);
  CHK_MALLOC(spectrum2,double,n_dest);
  CHK_MALLOC(flg2,int,n_dest);

  for(j=0;j<n_dest;j++) {
    *(wl_dest+j)=start+resolution*j;
  }
  getconveted(wl_dest, n_dest,
	      spectrum1,n_dest,
	      flg1, n_dest,
	      argv[ifile], start,end,resolution,
	      dont_normalize_exp_sec,
	      &xdim, &ydim, &NumFrames,
	      &flag_wlcen,&ierror);
  if(ierror!=0)
    {
      free(wl_dest);
      free(spectrum1);
      free(flg1);
      free(spectrum2);
      free(flg2);
      return EXIT_FAILURE;
    }
  getconveted(wl_dest, n_dest,
	      spectrum2,n_dest,
	      flg2, n_dest,
	      argv[ifile+1], start,end,resolution,
	      dont_normalize_exp_sec,
	      &xdim, &ydim, &NumFrames,
	      &flag_wlcen,&ierror);
  if(ierror!=0)
    {
      free(wl_dest);
      free(spectrum1);
      free(flg1);
      free(spectrum2);
      free(flg2);
      return EXIT_FAILURE;
    }
  
  CHK_MALLOC(spectrum_dest,double,n_dest);
  CHK_MALLOC(flg_dest,int,n_dest);
  glue2(n_dest, wl_dest, spectrum1, flg1, spectrum2, flg2,
	spectrum_dest, flg_dest);
  dump_spectrum2(n_dest,flg_dest,wl_dest,spectrum_dest);

  free(spectrum1);
  free(flg1);
  free(spectrum2);
  free(flg2);
  
  free(spectrum_dest);
  free(flg_dest);
  free(wl_dest);

    
  return EXIT_SUCCESS;
}
