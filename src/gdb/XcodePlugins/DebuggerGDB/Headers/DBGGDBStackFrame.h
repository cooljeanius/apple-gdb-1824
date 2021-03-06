/*
 *     Generated by class-dump 3.3.4 (64 bit).
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2011 by Steve Nygard.
 */

#import "DBGStackFrame.h"

#import "DBGStackFrameDelegate-Protocol.h"

@class DVTMapTable, NSArray, NSMutableArray, NSMutableSet, PBXLSStackFrame;

@interface DBGGDBStackFrame : DBGStackFrame <DBGStackFrameDelegate>
{
    PBXLSStackFrame *_PBXStackFrame;
    DVTMapTable *_expressionContainers;
    NSMutableSet *_observers;
    NSMutableArray *_retryExpressionBlocks;
    NSArray *_locals;
    NSArray *_arguments;
    NSArray *_fileStatics;
    NSArray *_registers;
}

+ (id)keyPathsForValuesAffectingLineNumber;
@property(copy, nonatomic) NSArray *registers; // @synthesize registers=_registers;
@property(copy, nonatomic) NSArray *fileStatics; // @synthesize fileStatics=_fileStatics;
@property(copy, nonatomic) NSArray *arguments; // @synthesize arguments=_arguments;
@property(copy, nonatomic) NSArray *locals; // @synthesize locals=_locals;
@property(retain, nonatomic) PBXLSStackFrame *PBXStackFrame; // @synthesize PBXStackFrame=_PBXStackFrame;
- (void)stackFrame:(id)arg1 requestDisassembly:(id)arg2;
- (void)_requestStackBasedContainerNamed:(id)arg1 completionBlock:(id)arg2;
- (id)_wrapPBXLSDataValues:(id)arg1;
- (void)requestDataValueForExpression:(id)arg1 atBlockStartAddress:(id)arg2 onQueue:(struct dispatch_queue_s *)arg3 withResultBlock:(id)arg4;
- (void)requestDataValueForSymbol:(id)arg1 symbolKind:(id)arg2 atLocation:(id)arg3 onQueue:(struct dispatch_queue_s *)arg4 withResultBlock:(id)arg5;
- (void)_requestDataValueForExpression:(id)arg1 atLocation:(id)arg2 orBlockStartAddress:(id)arg3 onQueue:(struct dispatch_queue_s *)arg4 withResultBlock:(id)arg5;
- (void)_watchChangeObject:(id)arg1 keyPath:(id)arg2 withChangeBlock:(id)arg3;
- (void)updateExpressionValues;
- (void)resetDataValues;
- (id)lineNumber;
- (BOOL)isEqual:(id)arg1;
- (id)initWithParentThread:(id)arg1 frameNumber:(id)arg2 framePointer:(id)arg3 name:(id)arg4;

@end

