//
//  FileDialogController.m
//  ViewSPE_cocoa
//
//  Created by Junichi Motohisa on 11/09/27.
//  Copyright 2011 Junichi Motohisa. All rights reserved.
//

#import "FileDialogController.h"
#import "WinSpecHeader25.h"
#import "SPEheader.h"
#import "SPEData.h"
#import "AutoTicks.h"

//#import "readspe.h"

@implementation FileDialogController
- (IBAction)onSelectOpen:(id)sender
{
    NSOpenPanel *openPanel	= [NSOpenPanel openPanel];
    NSArray *allowedFileTypes = [NSArray arrayWithObjects:@"spe",@"SPE",nil];
//	NSArray* fileTypes = [[NSArray alloc] initWithObjects:@"pdf", @"PDF", nil];
    [openPanel setAllowedFileTypes:allowedFileTypes];
    NSInteger pressedButton = [openPanel runModalForTypes:allowedFileTypes];
    
    if( pressedButton == NSOKButton ){
		
        // get file path and file name
//        NSURL * filePath = [openPanel URL];
		NSArray * folderPath = [openPanel filenames];
		NSString *fileName;
		fileName = [folderPath componentsJoinedByString:@"/"];
		NSString *fileName2;
		fileName2 = [folderPath lastObject];
		
        NSLog(@"file opened '%@'", fileName);
        NSLog(@"file '%@'", fileName2);
		
        // open and read data
		// read header
		SPEHeader *speHeader;
		speHeader = [[SPEHeader alloc] init];
//		WINXHEADER_STRUCT header0 = [speHeader read:fileName];
		header = [speHeader read:fileName];
		// read data
		SPEData *speData;
		speData = [[SPEData alloc] init];
		[speData read:fileName 
							headerName:header];
		
//		int i;
		x = [[NSMutableArray alloc] init];
		data = [[NSMutableArray alloc] init];
//		for(i=0;i<header.xdim;i++)
//		{
//			[x addObject:[speData->x objectAtIndex:i]];
//			[data addObject:[speData->data objectAtIndex:i]];
//		}
		x = [speData x];
		data = [speData data];
		
//		CPTXYGraph *graph;
		graph = [[[CPTXYGraph alloc] initWithFrame:_hostingView.bounds] autorelease];
		_hostingView.hostedLayer = graph;
		
		graph.paddingBottom = 20;
		graph.paddingLeft = 20;
		graph.paddingTop = 20;
		graph.paddingRight = 20;
		graph.plotAreaFrame.paddingLeft = 70;
		graph.plotAreaFrame.paddingBottom = 60;
		graph.plotAreaFrame.paddingRight = 20;
		graph.plotAreaFrame.paddingTop = 20;
//		graph.title = @"filename";
		graph.title = fileName2;
		
		CPTXYPlotSpace *plotSpace;
		plotSpace = (CPTXYPlotSpace *)graph.defaultPlotSpace;
//		plotSpace.xRange = [CPTPlotRange plotRangeWithLocation:CPTDecimalFromFloat(speData->xmin)
//														length:CPTDecimalFromFloat(speData->xmax)];
		plotSpace.xRange = [CPTPlotRange plotRangeWithLocation:CPTDecimalFromDouble([speData xMin])
														length:CPTDecimalFromDouble([speData xMax]-[speData xMin])];
		plotSpace.yRange = [CPTPlotRange plotRangeWithLocation:CPTDecimalFromDouble([speData yMin])
														length:CPTDecimalFromDouble([speData yMax]-[speData yMin])];
		double xTickInterval,yTickInterval;
		xTickInterval = majorTickInterval([speData xMin],[speData xMax]);
		yTickInterval = majorTickInterval([speData yMin],[speData yMax]);
		
		CPTMutableLineStyle *lineStyle = [CPTMutableLineStyle lineStyle];
		lineStyle.lineColor = [CPTColor blackColor];
		lineStyle.lineWidth = 2.0f;
		
		CPTXYAxisSet *axisSet = (CPTXYAxisSet *)graph.axisSet;
		CPTXYAxis *xAxis = axisSet.xAxis;
//		xAxis.labelingPolicy = CPTAxisLabelingPolicyFixedInterval;
		xAxis.majorIntervalLength = CPTDecimalFromDouble(xTickInterval);
		xAxis.minorTicksPerInterval = 0;
		xAxis.majorTickLineStyle = lineStyle;
		xAxis.minorTickLineStyle = lineStyle;
		xAxis.axisLineStyle = lineStyle;
		xAxis.minorTickLength = 5.0f;
		xAxis.majorTickLength = 9.0f;
//		xAxis.isFloatingAxis = YES;
		xAxis.orthogonalCoordinateDecimal = CPTDecimalFromDouble([speData yMin]);
		xAxis.title = @"wavelength";
		
		//	Y Axis
		CPTXYAxis *yAxis = axisSet.yAxis;
//		yAxis.labelingPolicy = CPTAxisLabelingPolicyFixedInterval;
		yAxis.majorIntervalLength = CPTDecimalFromDouble(yTickInterval);
		yAxis.minorTicksPerInterval = 0;
		yAxis.majorTickLineStyle = lineStyle;
		yAxis.minorTickLineStyle = lineStyle;
		yAxis.axisLineStyle = lineStyle;
		yAxis.minorTickLength = 5.0f;
		yAxis.majorTickLength = 9.0f;
//		yAxis.isFloatingAxis = YES;
		yAxis.orthogonalCoordinateDecimal = CPTDecimalFromDouble([speData xMin]);
		yAxis.title = @"intensity";
		yAxis.titleOffset = 35.0f;	//	move left from y axis. negative value is go right.
		lineStyle.lineWidth = 0.5f;
		yAxis.majorGridLineStyle = lineStyle;
		
		graph.axisSet.axes = [NSArray arrayWithObjects:xAxis, yAxis, nil];
		
		CPTScatterPlot *spePlot;
		spePlot = [[[CPTScatterPlot alloc] init] autorelease];
		spePlot.identifier = @"spe";
		CPTMutableLineStyle *ls1 = [CPTMutableLineStyle lineStyle];
		ls1.lineWidth = 2.0f;
		ls1.lineColor = [CPTColor redColor];
		spePlot.dataLineStyle= ls1;
		spePlot.dataSource = self;
		[graph addPlot:spePlot];
		
//		[x release];
//		[data release];
//		[speData release];
//		[speHeader release];
    }else if( pressedButton == NSCancelButton ){
     	NSLog(@"Cancel button was pressed.");
    }else{
     	// error
    }
//	[openPanel release];
}

- (NSUInteger)numberOfRecordsForPlot:(CPTPlot *)plot
{
//	return [NSNumber numberWithInt:header.xdim];
	NSUInteger xdim;
	xdim = [[NSNumber numberWithInt:header.xdim] unsignedIntValue];
	return xdim;
}

//
-(NSNumber *)numberForPlot:(CPTPlot *)plot field:(NSUInteger)fieldEnum recordIndex:(NSUInteger)index
{
    if (fieldEnum == CPTScatterPlotFieldX)
        return [self->x objectAtIndex:index];
	else
		return [self->data objectAtIndex:index];
}

@end
