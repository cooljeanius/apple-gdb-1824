/*
 * GuiGdbManagerInternal_Protocol.h
 * (This is an Objective-C header; i.e. NOT just-plain-C)
 */

/*
 * Methods the DebuggerController can invoke on the GdbManger.
 * Most invocations are from the Debugger Server's thread.
 */

@protocol GuiGdbManagerInternal  <NSObject>

- (void) setDebuggerControllerConnectionName:(NSString *)n;

/* Put a command in the queue; can block while waiting for a lock. */
- (void)enqueueCmd:(GdbCmd*)c;

/* synching with gdb */
- (void) setupForSynch;
- (void) waitForSynch;
@end

/* EOF */
