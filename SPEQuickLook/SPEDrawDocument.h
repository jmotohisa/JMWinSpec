//
//  SPEDrawDocument.h
//  SPEQuickLookLeopard
//
//  Created by Junichi Motohisa on 11/10/08.
//  Copyright 2011 Junichi Motohisa. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import <CorePlot/CorePlot.h>
#import "WinSpecHeader25.h"

//extern NSString *SPEDrawDocumentCanvasSizeKey;
//extern NSString *SPEDrawDocumentGraphicsKey;

@interface SPEDrawDocument : NSDocument <CPTScatterPlotDataSource>
{
	WINXHEADER_STRUCT header;
	NSMutableArray *x;
	NSMutableArray *data;

	NSMutableArray *_graphics;

//    NSMutableSet *_undoGroupInsertedGraphics;
//    NSMutableDictionary *_undoGroupOldPropertiesPerGraphic;
//    NSString *_undoGroupPresentablePropertyName;
//    BOOL _undoGroupHasChangesToMultipleProperties;
	
}

// Return the current value of the property.
- (NSSize)canvasSize;

- (CPTGraphHostingView *)drawSPEfile:(NSString *)fileName;

@end
