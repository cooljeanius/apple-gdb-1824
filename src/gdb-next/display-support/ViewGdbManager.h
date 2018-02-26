/* -*- Objective-C -*-
 * ViewGdbManager.h
 * (This is an Objective-C header; i.e. NOT just-plain-C)
 */

#import "GdbManager.h"
#import "DisplayMisc.h"
#import "ViewDisplayProvider_Protocol.h"
#import <Foundation/Foundation.h>

/*
 * A view display provider is pretty simple: we just send
 * it one-way messages telling it the current line.
 *
 * The ViewGdbManager makes the connection with the view
 * provider.
 */
@interface ViewGdbManager : GdbManager
{
}

- (void) dealloc;

- (int) establishConnection;


@end

extern GdbManager *make_view_gdb_manager(void);

/* EOF */
