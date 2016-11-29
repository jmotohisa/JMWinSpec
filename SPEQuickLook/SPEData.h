//
//  SPEData.h
//  ViewSPE_cocoa
//
//  Created by Junichi Motohisa on 11/10/01.
//  Copyright 2011 Junichi Motohisa. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <CorePlot/CorePlot.h>
#import "WinSpecHeader25.h"

@interface SPEData : NSObject
{
	double xmin,xmax,ymin,ymax,zmin,zmax;
//@public
	NSMutableArray *x;
	NSMutableArray *data;
}

-(void) read:(NSString *)fileName
      headerName:(WINXHEADER_STRUCT )header;

-(double ) xMin;
-(double ) xMax;
-(double ) yMin;
-(double ) yMax;
-(double ) zMin;
-(double ) zMax;

-(NSMutableArray *) data;
-(NSMutableArray *) x;

@end
