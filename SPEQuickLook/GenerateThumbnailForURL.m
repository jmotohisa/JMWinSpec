#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <QuickLook/QuickLook.h>

#include "SPEDrawDocument.h"

/* -----------------------------------------------------------------------------
    Generate a thumbnail for file

   This function's job is to create thumbnail for designated file as fast as possible
   ----------------------------------------------------------------------------- */

OSStatus GenerateThumbnailForURL(void *thisInterface, QLThumbnailRequestRef thumbnail, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options, CGSize maxSize)
{
//    #warning To complete your generator please implement the function GenerateThumbnailForURL in GenerateThumbnailForURL.m
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    
	
	NSString *filePath=[(NSURL *)url path];
//	NSSize *canvasSize;
	SPEDrawDocument *SPEDraw=[[SPEDrawDocument alloc] init];
	CPTGraphHostingView *imageData=[[CPTGraphHostingView alloc] init];
	imageData = [SPEDraw drawSPEfile:filePath];
	NSImage *image = [[imageData hostedGraph] imageOfGraph;
	CGImageRef imageDataRef = (CGImageRef)[image TIFFRepresentation];
	
	QLThumbnailRequestSetImage(thumbnail,imageDataRef,nil);
    
    [pool release];
    return noErr;
 }

void CancelThumbnailGeneration(void* thisInterface, QLThumbnailRequestRef thumbnail)
{
    // implement only if supported
}
