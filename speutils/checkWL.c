/*
 *  checkWL.c - Time-stamp: <Tue Nov 29 10:39:43 JST 2022>
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
 *  $Id: checkWL.c 2022-11-29 10:27:12 jmotohisa $
 */

/*! 
  @file checkWL.c 
  @brief 
  @author J. Motohisa
  @date
*/

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <tgmath.h>
#include <unistd.h>

#include "config.h"
#include "copyright.h"

#include "WinSpecHeader25.h"
#include "readspe.h"

/* #define GLOBAL_VALUE_DEFINE */
/* #include "checkWL.h" */

/*!
  @brief
  @param[in]
  @param[out]
  @param[in,out]
  @return
*/

void usage(FILE *f)
{
  fprintf(f, "Usage: checkWL [options] [<filenames>]\n"
		  "Options:\n"
		  "         -h : this help message\n"
		  "         -V : print version number and copyright\n"
		  "         -w : just warn difference\n"
		  );
}

int main(int argc, char **argv)
{
  WINXHEADER_STRUCT header;
  int c;
  int warn = 0;
  int err;
  int ifile;
  long i,n;
  double *data,*x;
  
  while ((c = getopt(argc, argv, "hVw")) != -1)
	switch (c) {
	case 'h':
	  usage(stdout);
	  return EXIT_SUCCESS;
	case 'V':
	  printf("checkWL " PACKAGE_VERSION " by Junichi Motohisa\n"
	  		 COPYRIGHT);
	  return EXIT_SUCCESS;
	case 'w':
	  warn = 1;
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
  
  for (ifile = optind; ifile < argc; ++ifile) {
	/* read data from SPE file*/
	/* if (verbose) { */
	/*   printf("reading from \"%s\"", argv[ifile]); */
	/* } */
	if(err = read_spe_header(argv[ifile],&header)>0)
	  {
	    n=header.xdim*header.ydim*header.NumFrames;
	    data = (double *) malloc(sizeof(double)*n);
	    x=(double *) malloc(sizeof(double)*header.xdim);
	  } else {
	  break;
	}
	err = read_spe_data(argv[ifile],data,header);
	poly((int) header.xdim, x, 6, header.polynom_coeff_x);

	double wlcen=header.SpecCenterWlNm;
	int xDimDet=header.xDimDet;
	double wlcen2 = poly0((1+xDimDet)/2.,6,header.polynom_coeff_x);
	if(warn==1)
	  {
	    if(fabs(wlcen-wlcen2)>1)
	      {
		printf("Calibration error in file %s: actutal %lf, but calibration is %lf\n", argv[ifile],wlcen,wlcen2);
	      }
	  }
	else
	  {
	    printf("%s:\t%lf\t%lf\n",argv[ifile],wlcen,wlcen2);
	  }
  }
}
