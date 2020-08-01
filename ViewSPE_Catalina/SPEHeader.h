//
//  SPEHeader.h
//  ViewSPE_cocoa
//
//  Created by Junichi Motohisa on 11/10/01.
//  Copyright 2011 北海道大学. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "WinSpecHeader25.h"

@interface SPEHeader : NSObject {
//	IBOutlet NSString *fileName;
//	IBOutlet WINXHEADER_STRUCT *header;
}

-(WINXHEADER_STRUCT)read:(NSString *)fileName;

@end
