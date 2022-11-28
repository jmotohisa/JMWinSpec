// readspe.c: read spe data

#include <stdio.h>
#include <stdlib.h>
/* #include <byteswap.h> */

#include "WinSpecHeader25.h"

#define GLOBAL_VALUE_DEFINE
#include "readspe.h"

int read_spe_header(char *filename, WINXHEADER_STRUCT *header)
{
  FILE *fp;
  if((fp=fopen(filename,"rb"))==NULL)
	{
	  printf("cannot open file :%s\n",filename);
	  return(-1);
	}
  //  size_t fread( void *buf, size_t size, size_t num, FILE *fp )
  fread(header, sizeof(WINXHEADER_STRUCT), 1, fp);
  fclose(fp);
  return(1);
}

int read_spe_data(char *filename, double *data, WINXHEADER_STRUCT header)
{
  FILE *fp;
  float *f_data;
  long *l_data;
  short *s_data;
  unsigned short *us_data;
  void *data0;
  long i;
  long n=header.xdim*header.ydim*header.NumFrames;
	
  if((fp=fopen(filename,"rb"))==NULL)
	{
	  printf("cannot open file :%s\n",filename);
	  return(-1);
	}
  // header.datatype
  //                 0 =   float (4 bytes)
  //                 1 =   long (4 bytes)
  //                 2 =   short (2 bytes)
  //                 3 =   unsigned short (2 bytes)
  fseek(fp,sizeof(WINXHEADER_STRUCT),SEEK_SET);
  //  size_t fread( void *buf, size_t size, size_t num, FILE *fp )
  //	dtype=fdatatype(datatype)
  //  if(header.NumFrames==1 && header.ydim==1)
  switch (header.datatype) {
  case 0 :
	data0 = (void *) malloc(sizeof(float)*n);
	fread(data0,sizeof(float),n,fp);
	for(i=0;i<n;i++)
	  *(data+i)=*((float *) data0+i);
	break;
  case 1:
	//		CHK_MALLOC(l_data,long,n);
	//		fread(l_data,sizeof(long),n,fp);
	data0 = (void *) malloc(sizeof(int)*n);
	fread(data0,sizeof(int),n,fp);
	for(i=0;i<n;i++)
	  {
		*(data+i)=*((int *) data0+i);
	  }
	break;
  case 2:
	data0 = (void *) malloc(sizeof(short)*n);
	fread(data0,sizeof(short),n,fp);
	for(i=0;i<n;i++)
	  *(data+i)=*((short *) data0+i);
	break;
  case 3:
	data0 = (void *) malloc(sizeof(unsigned short)*n);
	fread(data0,sizeof(unsigned short),n,fp);
	for(i=0;i<n;i++)
	  *(data+i)=*((unsigned short *) data0+i);
	break;
  deafult:
	printf("invalid datatype: %d . execution aborted.\n",header.datatype);
	fclose(fp);
	return(1);
	break;
  }
  fclose(fp);
  free(data0);
  return(1);
}
  
int poly(int n, double *x, int norder, double *coef)
{
  int i,j;
  double y;
  
  for(i=0;i<n;i++)
	{
//	  *(x+i)=*(coef+norder-1);
	  y=*(coef+norder-1);
	  for(j=norder-2;j>=0;j--)
		{
		  //		  *(x+i)=*(coef+j)+(i+1)*(*(x+i));
		  y=*(coef+j)+(i+1)*y; // note: should be i+1,  not i
		}
	  *(x+i)=y;
	}
  return(0);
}

