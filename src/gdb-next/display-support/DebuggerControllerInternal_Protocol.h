/* -*- Objective-C -*-
 * DebuggerControllerInternal_Protocol.h
 * (This is an Objective-C header; i.e. NOT just-plain-C)
 */

/* Protocol between the Gdb Manager object (and thread)
 * the DebuggerController. */

@protocol DebuggerControllerInternal <NSObject>

/* return the display provider to gdb */

- (id) displayProvider;

@end

/* EOF */
