//
//  SPEData.m
//  ViewSPE_cocoa
//
//  Created by Junichi Motohisa on 11/10/01.
//  Copyright 2011 Hokkaido University. All rights reserved.
//

#import "SPEData.h"
#import "readspe.h"

@implementation SPEData

-(void) read:(NSString *)fileName
	  headerName:(WINXHEADER_STRUCT)header
{
	double *x0,*data0;
	x0=malloc(sizeof(double)*header.xdim);
	data0=malloc(sizeof(double)*header.xdim*header.ydim*header.NumFrames);
	read_spe_data((char *) [fileName UTF8String], data0,header);
	poly((int) header.xdim, x0,6, header.polynom_coeff_x);
	xmin=*x0;
	xmax=*(x0+header.xdim-1);
	
// substitute x to, data0 to data
	int i;
	x =[[NSMutableArray alloc] init];
	for(i=0;i<header.xdim;i++)
	{
		[x addObject:[NSNumber numberWithDouble:*(x0+i)]];
	}
	
	zmin=1e30;
	zmax=-1e30;
	data =[[NSMutableArray alloc] init];
	for(i=0;i<header.xdim*header.ydim*header.NumFrames;i++)
	{
		if(zmin>*(data0+i))
			zmin=*(data0+i);
		if(zmax<*(data0+i))
			zmax=*(data0+i);
		[data addObject:[NSNumber numberWithDouble:*(data0+i)]];
	}
	if(header.NumFrames==1)
	{
		ymin=zmin;
		ymax=zmax;
	}
	else
	{
		ymin=0;
		ymax=header.ydim;
	}

	free(x0);
	free(data0);
}

-(double ) xMin
{
	return xmin;
}

-(double) xMax
{
	return xmax;
}

-(double ) yMin
{
	return ymin;
}

-(double) yMax
{
	return ymax;
}
-(double) zMin
{
	return zmin;
}

-(double) zMax
{
	return zmax;
}

-(NSMutableArray *) data
{
	return data;
}

-(NSMutableArray *) x
{
	return(x);
}

@end
