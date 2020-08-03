//
//  SPEDrawDocument.m
//  SPEQuickLookLeopard
//
//  Created by Junichi Motohisa on 11/10/08.
//  Copyright 2011 北海道大学. All rights reserved.
//

#import "SPEDrawDocument.h"
#import <CorePlot/CorePlot.h>
#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>

#import "WinSpecHeader25.h"
#import "SPEData.h"
#import "SPEHeader.h"
#import "AutoTicks.h"
//#import <objc/objc-runtime.h>

//NSString *SPEDrawDocumentCanvasSizeKey = @"canvasSize";
//NSString *SPEDrawDocumentGraphicsKey = @"graphics";

/*
// Values that are used as contexts by this class' invocation of KVO observer registration methods. See the comment near the top of SPEGraphicView.m for a discussion of this.
static NSString *SPEDrawDocumentUndoKeysObservationContext = @"com.apple.SPEDrawDocument.undoKeys";
static NSString *SPEDrawDocumentUndoObservationContext = @"com.apple.SPEDrawDocument.undo";

// The document type names that must also be used in the application's Info.plist file. We'll take out all uses of SPEDrawDocumentOldTypeName and SPEDrawDocumentOldVersion1TypeName (and NSPDFPboardType and NSTIFFPboardType) someday when we drop 10.4 compatibility and we can just use UTIs everywhere.
static NSString *SPEDrawDocumentOldTypeName = @"Apple Sketch document";
static NSString *SPEDrawDocumentNewTypeName = @"com.apple.sketch2";
static NSString *SPEDrawDocumentOldVersion1TypeName = @"Apple Sketch 1 document";
static NSString *SPEDrawDocumentNewVersion1TypeName = @"com.apple.sketch1";

// More keys, and a version number, which are just used in Sketch's property-list-based file format.
static NSString *SPEDrawDocumentVersionKey = @"version";
static NSString *SPEDrawDocumentPrintInfoKey = @"printInfo";
static int SPEDrawDocumentCurrentVersion = 2;
*/

// Some methods are invoked by methods above them in this file.
//@interface SPEDrawDocument(SPEForwardDeclarations)
//- (NSArray *)graphics;
//- (void)startObservingGraphics:(NSArray *)graphics;
//- (void)stopObservingGraphics:(NSArray *)graphics;
//@end

@implementation SPEDrawDocument

- (CPTGraphHostingView *)drawSPEfile:(NSString *)fileName
{
	// get file path and file name
	//        NSURL * filePath = [openPanel URL];
	//	NSArray * folderPath = [openPanel filenames];
	
	NSLog(@"file opened '%@'", fileName);
	//	NSLog(@"file '%@'", fileName2);
	
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
	
	if(header.NumFrames==1 && header.ydim==1)
	{
	// plot of single spectrum data
    NSRect viewRect;
	viewRect.origin.x=0;
	viewRect.origin.y=0;
	viewRect.size.width=501;
	viewRect.size.height=306;
//	NSView *view=[[NSView alloc] initWithFrame:viewRect] autorelease];
	CPTGraphHostingView *hostingView=[[CPTGraphHostingView alloc] initWithFrame:viewRect];
	CPTXYGraph *graph;
//	[hostingView setFrame:viewRect];
//	[hostingView setBounds:viewRect];
//	[view setBounds:viewRect];
	graph = [[[CPTXYGraph alloc] initWithFrame:viewRect] autorelease];
	//	graph = [[CPTXYGraph alloc] initWithFrame:hostingView.bounds];
	hostingView.hostedGraph = graph;
	
	graph.paddingBottom = 20;
	graph.paddingLeft = 20;
	graph.paddingTop = 20;
	graph.paddingRight = 20;
	graph.plotAreaFrame.paddingLeft = 70;
	graph.plotAreaFrame.paddingBottom = 60;
	graph.plotAreaFrame.paddingRight = 20;
	graph.plotAreaFrame.paddingTop = 20;
	//		graph.title = @"filename";
	graph.title = fileName;
	
	CPTXYPlotSpace *plotSpace;
	plotSpace = (CPTXYPlotSpace *)graph.defaultPlotSpace;
	//		plotSpace.xRange = [CPTPlotRange plotRangeWithLocation:CPTDecimalFromFloat(speData->xmin)
	//														length:CPTDecimalFromFloat(speData->xmax)];
        plotSpace.xRange = [CPTPlotRange plotRangeWithLocation:[NSNumber numberWithDouble:[speData xMin]]
                                                        length:[NSNumber numberWithDouble:([speData xMax]-[speData xMin])]];
        plotSpace.yRange = [CPTPlotRange plotRangeWithLocation:[NSNumber numberWithDouble:[speData yMin]]
                                                        length:[NSNumber numberWithDouble:([speData yMax]-[speData yMin])]];
	double xTickInterval,yTickInterval;
	xTickInterval = majorTickInterval([speData xMin],[speData xMax]);
	yTickInterval = majorTickInterval([speData yMin],[speData yMax]);
	
	CPTMutableLineStyle *lineStyle = [CPTMutableLineStyle lineStyle];
	lineStyle.lineColor = [CPTColor blackColor];
	lineStyle.lineWidth = 2.0f;
	
	CPTXYAxisSet *axisSet = (CPTXYAxisSet *)graph.axisSet;
	CPTXYAxis *xAxis = axisSet.xAxis;
	//		xAxis.labelingPolicy = CPTAxisLabelingPolicyFixedInterval;
    xAxis.majorIntervalLength = [NSNumber numberWithDouble:xTickInterval];
	xAxis.minorTicksPerInterval = 0;
	xAxis.majorTickLineStyle = lineStyle;
	xAxis.minorTickLineStyle = lineStyle;
	xAxis.axisLineStyle = lineStyle;
	xAxis.minorTickLength = 5.0f;
	xAxis.majorTickLength = 9.0f;
	//		xAxis.isFloatingAxis = YES;
    xAxis.orthogonalPosition = [NSNumber numberWithDouble:[speData yMin]];
	xAxis.title = @"wavelength";
	
	//	Y Axis
	CPTXYAxis *yAxis = axisSet.yAxis;
	//		yAxis.labelingPolicy = CPTAxisLabelingPolicyFixedInterval;
    yAxis.majorIntervalLength = [NSNumber numberWithDouble:yTickInterval];
	yAxis.minorTicksPerInterval = 0;
	yAxis.majorTickLineStyle = lineStyle;
	yAxis.minorTickLineStyle = lineStyle;
	yAxis.axisLineStyle = lineStyle;
	yAxis.minorTickLength = 5.0f;
	yAxis.majorTickLength = 9.0f;
	//		yAxis.isFloatingAxis = YES;
    yAxis.orthogonalPosition = [NSNumber numberWithDouble:[speData xMin]];
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
	
	[speHeader release];
	[speData release];

	return hostingView;
	} else if(header.NumFrames==1 && header.ydim != 1) {
		// image mode: data
		// ビットマップデータを取得する
		CFDataRef   dataRef;
		UInt8	*buffer;
		dataRef = CGDataProviderCopyData(dataProvider);
		buffer = (UInt8*)CFDataGetBytePtr(dataRef);
		
		// ビットマップに効果を与える
		NSUInteger  i, j;
		for (j = 0; j < [data yMax]; j++) {
			for (i = 0; i < [data xMax]; i++) {
				// ピクセルのポインタを取得する
				UInt8*  tmp;
				tmp = buffer + j * bytesPerRow + i * 4;
								
				// 輝度値を計算する
				UInt8   y;
				y = (*(data+j*xMax+i)-[data zMin])/([data zMax]-[data zMin]);
				
				// 輝度の値をRGB値として設定する
				*(tmp + 1) = y;
				*(tmp + 2) = y;
				*(tmp + 3) = y;
			}
		}
		
		// 効果を与えたデータを作成する
		CFDataRef   effectedData;
		effectedData = CFDataCreate(NULL, buffer, CFDataGetLength(dataRef));
		
		// 効果を与えたデータプロバイダを作成する
		CGDataProviderRef   effectedDataProvider;
		effectedDataProvider = CGDataProviderCreateWithCFData(effectedData);
		
		// 画像を作成する
		CGImageRef  effectedCgImage;
		CIImage*    effectedImage;
		effectedCgImage = CGImageCreate(
										header.xdim, header.ydim, 
										bitsPerComponent, bitsPerPixel, bytesPerRow, 
										colorSpace, bitmapInfo, effectedDataProvider, 
										NULL, shouldInterpolate, intent);
		effectedImage = [[UIImage alloc] initWithCGImage:effectedCgImage];
		[effectedImage autorelease];
		
		// 画像を表示する
		_imageView.image = effectedImage;
		
		// 作成したデータを解放する
		CGImageRelease(effectedCgImage);
		CFRelease(effectedDataProvider);
		CFRelease(effectedData);
		CFRelease(dataRef);
		
	} else if(header.numFrames!=1 && header.ydim ==1) {
		// multiple spectra mode
	}
	
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

- (NSSize)canvasSize {
    
    // A Sketch's canvas size is the size of the piece of paper that the user selects in the Page Setup panel for it, minus the document margins that are set.
    NSPrintInfo *printInfo = [self printInfo];
    NSSize canvasSize = [printInfo paperSize];
    canvasSize.width -= ([printInfo leftMargin] + [printInfo rightMargin]);
    canvasSize.height -= ([printInfo topMargin] + [printInfo bottomMargin]);
    return canvasSize;
    
}

@end
