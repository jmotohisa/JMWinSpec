/*
 *  glue.c - Time-stamp: <Fri Dec 02 20:06:31 JST 2022>
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

#define T00 0
#define T11 11
#define T22 22
#define T33 33
#define T01 1
#define T02 2
#define T03 3
#define T10 10
#define T12 12
#define T13 13
#define T20 20
#define T21 21
#define T23 23
#define T30 30
#define T31 31
#define T32 32

#define CHECK(cond, msg) { if (!(cond)) { fprintf(stderr, "glue error: %s\n", msg); exit(EXIT_FAILURE); } }

#define CHK_MALLOC(p, t, n) {                                         \
	size_t CHK_MALLOC_n_tmp = (n);									  \
	(p) = (t *) malloc(sizeof(t) * CHK_MALLOC_n_tmp);				  \
	CHECK((p) || CHK_MALLOC_n_tmp == 0, "out of memory!");			  \
  }

/*!
  @brief Calcuatate Calibrated wavlength from 
  @param[in] x: point number
  @return calibraated wavelength value
*/

void convert0(double *coef, int n_coef, double *spectrum_orig, int xdim,
	      double start, double end, double resolution,
	      double *wl_dest,int n_dest,
	      double *spectrum_dest,int *flg)
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
  pixel_start = (int *) malloc(sizeof(int)*n_dest);
  pixel_end = (int *) malloc(sizeof(int)*n_dest);
  
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
		  *(flg+j)=TRUE;
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
		  *(flg+j)=FALSE;
		  p=0;
		}
      *(spectrum_dest+j)=p;
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
  *spectrum_dest = (double *) malloc(sizeof(double)*(*n_dest));
  *flg = (int *) malloc(sizeof(int)*(*n_dest));
  
  for(j=0;j<*n_dest;j++) {
    *(*wl_dest+j)=start+resolution*j;
  }
  convert0(coef, n_coef, spectrum_orig, xdim,
	   start, end, resolution,
	   *wl_dest,*n_dest,*spectrum_dest,*flg);
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

#define S0 0
#define S1 1
#define S2 2
#define TR12 3
#define TR21 4
#define T2IN1 5
#define T1IN2 6

void findstartendpoints(int n, int *flg,
			int *nstart, int *start,
			int *nend,  int *end)
{
  int i,dflg;
  *nstart=0;
  *nend=0;
  if(*flg==1) {
    *start=0;
    (*nstart)++;
  }
  for(i=1;i<n;i++)
    {
      dflg=*(flg+i)-*(flg+i-1);
      if(dflg==1)
	{
	  *(start+*nstart)=i;
	  (*nstart)++;
	}
      if(dflg==-1)
	{
	  *(end+*nend)=i-1;
	  (*nend)++;
	}
    }
  if(*(flg+n-1)==1) {
    *(end+*nend)=n-1;
    (*nend)++;
  }
  return;
}

void glue2(int n, double *wl, double *spec1, int *flg1, double *spec2, int *flg2,
	   double *spec_dest, int *flg_dest)
{
  int i,glue_type;
  int *flg_glue,*flg_glue2;
  int glue_start=1;
  int pix_start1, pix_end1, pix_start2,pix_end2;
  flg_glue=(int *) malloc(sizeof(int)*n);
  int ss1, ss2;
  double *weight1,*weight2,weight0;
  int weight;
  int i0,ii;
  int prev,count;
  int *start1,*end1, *start2, *end2;
  int nstart1,nend1,nstart2, nend2;
  int p1, p2;
  int s1,e1,s2,e2;
  double d;

 for(i=0;i<n;i++) {
    *(spec_dest + i) = *(spec1+i)*(*(flg1+i)) + *(spec2+i)*(*(flg2+i));
    *(flg_dest + i) = *(flg1+i) || *(flg2+i);
 }

 CHK_MALLOC(start1,int,n);
 CHK_MALLOC(end1,int,n);
 CHK_MALLOC(start2,int,n);
 CHK_MALLOC(end2,int,n);
  // find start and end points of the spectra
  findstartendpoints(n,flg1,&nstart1, start1, &nend1, end1);
  findstartendpoints(n,flg2,&nstart2, start2, &nend2, end2);
  // debug
  for(i=0;i<nstart1;i++)
	{
	  printf("(%d, %d) \t",*(start1+i),*(end1+i));
	}
  printf("\n");
  for(i=0;i<nstart2;i++)
	{
	  printf("(%d, %d) \t",*(start2+i),*(end2+i));
	}
  printf("\n");
  for(i=0;i<n;i++)
	{
	  printf("%d:%lf\t%d\t%lf\t%d\t%lf\n",i,*(wl+i),*(flg1+i),*(spec1+i),*(flg2+i),*(spec2+i));
	}

  p1=0;
  p2=0;
  do {
  s1=*(start1+p1);
  e1=*(end1+p1);
  s2=*(start2+p2);
  e2=*(end2+p2);
  printf("(%d, %d) - (%d, %d)\n",s1,e1,s2,e2);
  if(s1<=s2 && e1<e2) {
    ii=1;
    d=(e1-s2)+2;
	printf("s1(%d)-s2(%d)-e1(%d)-e2(%d)\n",s1,s2,e1,e2);
	printf("delta=%f\n",d);
    for(i=s2;i<=e1;i++) {
	  printf("weight1=%f\n",1-ii/d);
      *(spec_dest + i) = *(spec1+i)*(*(flg1+i))*(1-(double )ii/d)
		+ *(spec2+i)*(*(flg2+i)) *( ((double ) ii/d));
      ii++;
    }
    p1++;
	continue;
  }
  if(s1>=s2 && e1>e2) {
    ii=1;
    d=(e2-s1)+2;
	printf("s2(%d)-s1(%d)-e2(%d)-e1(%d)\n",s2,s1,e2,e1);
	printf("delta=%f\n",d);
    for(i=s1;i<=e2;i++) {
	  printf("weight1=%f\n",ii/d);
      *(spec_dest + i) = *(spec1+i)*(*(flg1+i))*((double )ii/d)
	+ *(spec2+i)*(*(flg2+i)) *(1-(double ) ii/d);
      ii++;
    }
    p2++;
	continue;
  }
  if(s1<s2 && e1>=e2) {
	printf("s1(%d)-s2(%d)-e2(%d)-e1(%d)\n",s1,s2,e2,e1);
    for(i=s1;i<e1;i++)
      {
		*(spec_dest + i) = (*(spec1+i)*(*(flg1+i))
							+ *(spec2+i)*(*(flg2+i)))*0.5;
      }
    p1++;
	continue;
  }
  
  if(s1>s2 && e1<=e2) {
	printf("s2(%d)-s1(%d)-e1(%d)-e2(%d)\n",s2,s1,e1,s2);
    for(i=s2;i<e2;i++)
      {
		*(spec_dest + i) = (*(spec1+i)*(*(flg1+i))
							+ *(spec2+i)*(*(flg2+i))) *0.5;
      }
    p2++;
	continue;
  }
  
  } while(p1<nstart1 && p2<nstart2);
  free(start1);
  free(end1);
  free(start2);
  free(end2);
	
}

void check_pix_start_end(int n, int ss, int *flg, int *start, int *end)
{
  int i;
  *start=-1;
  *end=-1;
  
  for(i=ss;i<n-1;i++)
    {
      if(*(flg+i)==0 && *(flg+i+1)==1)
	{
	  *start=i+1;
	  break;
	}
    }
  if(*(flg+ss)==1)
    *start=0;

  for(i=ss;i<n-1;i++)
    {
      if(*(flg+i)==1 && *(flg+i+1)==0)
	{
	  *end=i;
	  break;
	}
    }
  if(*(flg+n-1)==1)
    *end=n-1;

  return;
}
    
	
	
      
    
