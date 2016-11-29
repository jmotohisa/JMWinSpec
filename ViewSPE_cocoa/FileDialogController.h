//
//  FileDialogController.h
//  ViewSPE_cocoa
//
//  Created by Junichi Motohisa on 11/09/27.
//  Copyright 2011 Junichi Motohisa. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import <CorePlot/CorePlot.h>
#import "WinSpecHeader25.h"

@interface FileDialogController : NSObject <CPTScatterPlotDataSource>
{
//	IBOutlet NSString *fileName;
    IBOutlet CPTLayerHostingView *_hostingView;
    CPTXYGraph *graph;
//@private
	WINXHEADER_STRUCT header;
	NSMutableArray *x;
	NSMutableArray *data;
}

- (IBAction)onSelectOpen:(id)sender;

@end
