#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <QuickLook/QuickLook.h>

#import <CorePlot/CorePlot.h>

#import "WinSpecHeader25.h"
#import "SPEheader.h"
#import "SPEData.h"
#import "AutoTicks.h"
#import "SPEDrawDocument.h"
#import <CorePlot/CorePlot.h>

/* -----------------------------------------------------------------------------
   Generate a preview for file

   This function's job is to create preview for designated file
   ----------------------------------------------------------------------------- */

OSStatus GeneratePreviewForURL(void *thisInterface, QLPreviewRequestRef preview, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options)
{
//    #warning To complete your generator please implement the function GeneratePreviewForURL in GeneratePreviewForURL.m
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
//    SPEDrawDocument* document = [[SPEDrawDocument alloc] init];
	
	NSString *filePath=[(NSURL *)url path];
//	NSSize *canvasSize;
	SPEDrawDocument *SPEDraw=[[SPEDrawDocument alloc] init];
	CPTGraphHostingView *imageData=[[CPTGraphHostingView alloc] init];
	imageData = [SPEDraw drawSPEfile:filePath];
	NSImage *image = [[imageData hostedGraph] imageOfLayer];
	CFDataRef imageDataRef = (CFDataRef)[image TIFFRepresentation];
	
	QLPreviewRequestSetDataRepresentation(preview,imageDataRef,kUTTypeImage,nil);
	
	[filePath release];
	[SPEDraw release];
	[imageData release];
// 	[image release];
    [pool release];
    return noErr;
}

void CancelPreviewGeneration(void* thisInterface, QLPreviewRequestRef preview)
{
    // implement only if supported
}

