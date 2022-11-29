/* viewspe.c
 * Simple viewer of SPE files
 * display with plplot
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "copyright.h"

#ifdef HAS_PLPLOT
#include <plplot.h>
#include "plplotsubs0.h"
#endif

#include "WinSpecHeader25.h"
#include "readspe.h"

#ifdef AQUATERM
#define TERM "aqt"
#else
#define TERM "xwin"
#endif

#define CHECK(cond, msg) { if (!(cond)) { fprintf(stderr, "viewspe error: %s\n", msg); exit(EXIT_FAILURE); } }

#define CHK_MALLOC(p, t, n) {                                         \
	size_t CHK_MALLOC_n_tmp = (n);									  \
	(p) = (t *) malloc(sizeof(t) * CHK_MALLOC_n_tmp);				  \
	CHECK((p) || CHK_MALLOC_n_tmp == 0, "out of memory!");			  \
  }

//int read_spe_data(char *, double *, WINXHEADER_STRUCT );

void usage(FILE *f)
{
  fprintf(f, "Usage: viewspe [options] [<filenames>]\n"
		  "Options:\n"
		  "         -h : this help message\n"
		  "         -V : print version number and copyright\n"
		  "         -v : verbose output\n"
		  "         -g : display graph\n"
	          "         -d : dump spectrum\n"
	          "         -D : dump spectrum (with filename)\n"
		  "         -i : show image plot\n"
		  "         -l : diplay with axis label\n"
		  "         -n : normalize with exposure time\n"
		  );
}

int main(int argc, char **argv)
{
  WINXHEADER_STRUCT header;
  int c;
  int verbose = 0;
  int image = 0;
  int label = 0;
  int fdump = 0;
  int normalize = 0;
  int graph=0;
  int err;
  int ifile;
  long i,n;
  double *data,*x;
  
  while ((c = getopt(argc, argv, "hVvildng")) != -1)
	switch (c) {
	case 'h':
	  usage(stdout);
	  return EXIT_SUCCESS;
	case 'V':
	  printf("viewspe " PACKAGE_VERSION " by Junichi Motohisa\n"
	  		 COPYRIGHT);
	  return EXIT_SUCCESS;
	case 'v':
	  verbose = 1;
	  break;
	case 'i':
	  image = 1;
	  break;
	case 'l':
	  label = 1;
	  break;
	case 'd':
	  fdump = 1;
	  break;
	case 'D':
	  fdump = 2;
	  break;
	case 'n':
	  normalize = 1;
	  break;
	case 'g':
	  graph = 1;
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
	if (verbose) {
	  printf("reading from \"%s\"", argv[ifile]);
	}
	if(err = read_spe_header(argv[ifile],&header)>0)
	  {
	//malloc(sizeof(t) * CHK_MALLOC_n_tmp)
	    n=header.xdim*header.ydim*header.NumFrames;
	    data = (double *) malloc(sizeof(double)*n);
	    x=(double *) malloc(sizeof(double)*header.xdim);
	  } else {
	  break;
	}
	err = read_spe_data(argv[ifile],data,header);
	//	for(i=0;i<header.xdim;i++)
	//	  *(x+i)=i;
	//	GBLoadWave/Q/N=$"coef"/T={4,4}/B/U=6/S=3263/W=1/P=$path file

	//	poly((int) header.xdim, x,atoi(header.polynom_order_x), header.polynom_coeff_x);
	poly((int) header.xdim, x, 6, header.polynom_coeff_x);

	if(normalize==1)
	  {
	    for(i=0;i<n;i++)
	      *(data+i)=*(data+i)/header.exp_sec;
	  }
	switch (fdump) {
	case 1:
	  dump_spectrum("",header.xdim,x,data); // hear 1D data is assumed now
	  break;
	case 2:
	  dump_spectrum(argv[ifile],header.xdim,x,data); // hear 1D data is assumed now
	  break;
	case 0:
	default:
	  break;
	}
	  
	// plot
#ifdef HAS_PLPLOT
	if(graph==1)
	  {
	PLFLT *x_,*data_;
	data_ = (double *) malloc(sizeof(double)*n);
	x_=(double *) malloc(sizeof(double)*header.xdim);
	for(i=0;i<header.xdim;i++)
	  *(x_+i)=*(x+i);
	for(i=0;i<header.xdim*header.ydim*header.NumFrames;i++)
	  *(data_+i)=*(data+i);
	plsdev(TERM);
	plinit();
	if(header.ydim==1 & header.NumFrames==1)
	  plot1_plplot(header.xdim,x_,data_,argv[ifile]); // 1d plot
	else
	  {
	    if(header.NumFrames ==1 )
	      { // 2D plot
		PLFLT *y;
		int j;
		y=(PLFLT *) malloc(sizeof(PLFLT)*header.ydim*header.NumFrames);
		for(j=0;j<header.ydim*header.NumFrames;j++)
		  *(y+j)=j;
		//		plot2d_plsurf3d_plplot(header.xdim,header.ydim*header.NumFrames,
		//						 data,argv[ifile]); // surface plot
		plot2d_plshade_plplot(header.xdim,header.ydim*header.NumFrames,
				      x_,y,
				      data_, argv[ifile]); // surface plot
		free(y);
	      }
	    else if(header.ydim==1)
	      { // 1D multiple plot
		plot1_plplot2(header.xdim,header.NumFrames,x_,data_,argv[ifile]); // 1d plot
	      }
	  }
	plend();
	free(x_);
	free(data_);
	  }
#endif

	free(data);
	free(x);
  }
  return EXIT_SUCCESS;
}

