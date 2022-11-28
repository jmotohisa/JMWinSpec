/*
 *  convertspe.c - Time-stamp: <Mon Nov 28 21:14:24 JST 2022>
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
 *  $Id: convertspe.c 2022-11-27 13:28:39 jmotohisa $
 */

/*! 
  @file convertspe.c 
  @brief 
  @author J. Motohisa
  @date
*/

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <tgmath.h>
#include <unistd.h>

#include "../ViewSPE/WinSpecHeader25.h"

#define GLOBAL_VALUE_DEFINE
#include "convertspe.h"
#include "glue.h"

/*!
  @brief
  @param[in]
  @param[out]
  @param[in,out]
  @return
*/
void dump_spectrum(char *s, int n, double *wl, double *spectrum);
void dump_spectrum2(int n, int *flg, double *wl, double *spectrum);

void usage(FILE *f)
{
  fprintf(f, "Usage: convertspe [options] [<filenames>]\n"
		  "Options:\n"
		  "         -h : this help message\n"
		  "         -s  <val> : starting waveleng\n"
		  "         -e  <val> : ending waveleng\n"
		  "         -r  <val> : resolution\n"
		  "         -c : check wlcen\n"
		  "         -n : dont normalize with exp_sec\n"
		  
		  );
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
  double *data,*x;
  double start,end,resolution;
  double *spectrum_dest,*wl_dest;
  int n_dest,*flg;
  int check_SpecCenterWlNm,dont_normalize_exp_sec;
  
  while ((c = getopt(argc, argv, "hs:e:r:cn")) != -1)
	switch (c) {
	case 'h':
	  usage(stdout);
	  return EXIT_SUCCESS;
	case 's':
	  start=atof(optarg);
	  break;
	case 'e':
	  end=atof(optarg);
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
  
  if (optind == argc) {  /* no parameters left */
	usage(stderr);
	return EXIT_FAILURE;
  }

  double *coef, *wl, *spectrum;
  int nxdim,nydim,numFrames;
  int n_coef=6;

  for (ifile = optind; ifile < argc; ++ifile)
    {
      getspe(argv[ifile], &coef, &wl, &spectrum, &nxdim, &nydim, &numFrames);
      convert(coef, n_coef, spectrum, nxdim,
	      start, end, resolution,
	      &spectrum_dest,&flg, &wl_dest, &n_dest);
	  dump_spectrum2(n_dest,flg,wl_dest,spectrum_dest);
	  free(flg);
	  free(wl_dest);
	  free(spectrum_dest);
    }
  
}

void dump_spectrum2(int n, int *flg, double *wl,double *spectrum)
{
int i;
for(i=0;i<n;i++) {
  printf("%d\t%lf\t%lf\n",*(flg+i), *(wl+i),*(spectrum+i));
}
return;
}
