//
//  SPEHeader.m
//  ViewSPE_cocoa
//
//  Created by Junichi Motohisa on 11/10/01.
//  Copyright 2011 北海道大学. All rights reserved.
//

#import "SPEHeader.h"
#import "readspe.h"

@implementation SPEHeader
- (WINXHEADER_STRUCT)read:(NSString *)fileName;
{
	WINXHEADER_STRUCT header;
	read_spe_header((char *) [fileName UTF8String], &header);
	return header;
//		double *data;
//		data = (double *) malloc(sizeof(double)*header.xdim*header.ydim*header.NumFrames);
//		read_spe_data((char *) [fileName UTF8String], data,header);
}
@end
