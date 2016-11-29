/*
 *  AutoTicks.c
 *  ViewSPE_cocoa
 *
 *  Created by Junichi Motohisa on 11/10/03.
 *  Copyright 2011 Junichi Motohisa. All rights reserved.
 *
 */

#include "AutoTicks.h"
#include <math.h>

double majorTickInterval(double xmin, double xmax)
{
	double dx,dx2,logDx;
	double width;
	int exponent;
	double fraction;
	
	if ( xmin == xmax )
	{
			xmin--; xmax++;
	}
	dx = fabs(xmax - xmin);
	dx2 = dx/5; /* approximate width of tickmark */
	logDx = log10(dx2);
	exponent = floor(logDx);
	fraction = logDx - exponent;
	if(fraction < log10(2))
	{
		width = 1;
	}
	else if( fraction < log10(5))
	{
		width = 2;
	}
	else
	{
		width = 5;
	}
		
	width = width *pow(10,exponent);
	return(width);			
}
	

