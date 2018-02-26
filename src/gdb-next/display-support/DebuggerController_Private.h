/* -*- Objective-C -*-
 * DebuggerController_Private.h
 * (This is an Objective-C header; i.e. NOT just-plain-C)
 */

#import <Foundation/Foundation.h>
#import "DebuggerController_Protocol.h"
#import "GdbManager.h"
#import "GuiGdbCmd.h"
#import "GuiGdbManagerInternal_Protocol.h"
#import "GuiGdbManagerExecLock_Protocol.h"
#import "GuiDisplayProvider_Protocol.h"
#import "DebuggerControllerInternal_Protocol.h"

/*
 * DebuggerController established the connection to the remote
 * client which provides the display and GUI capablity. The
 * remote client can invoke commands and control gdb via the
 * DebuggerController protocol.
 *
 * The DebuggerController is a RunLoop.
 */

@interface DebuggerController : NSObject
	<DebuggerController, DebuggerControllerInternal>
{
    /* We call into this object to control and communicate w/GDB */
    GdbManager<GuiGdbManagerInternal, GuiGdbManagerExecLock>
        		   *gdbManager;

    /* object which provides visual feedback */
    id<GuiDisplayProvider>	displayProvider;

    NSConnection		*displayProviderConnection;
    NSConnection		*gdbConnection;
}

- (void) startController: gm;
- (void) handleConnectionDeath: (NSNotification *) notification;

@end

/* EOF */
