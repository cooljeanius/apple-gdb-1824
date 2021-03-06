/*
 *     Generated by class-dump 3.3.4 (64 bit).
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2011 by Steve Nygard.
 */

#import "NSObject.h"

@class NSString, PBXGDB_OpResult, PBXGDB_SyntheticDataValue;

@interface PBXGDB_EvalOperation : NSObject
{
    id _finishTarget;
    SEL _finishAction;
    PBXGDB_SyntheticDataValue *_syntheticDataValue;
    NSString *_expression;
    PBXGDB_OpResult *_result;
    NSString *_resultName;
}

- (void)perform;
- (id)result;
- (void)setResult:(id)arg1;
- (void)didFinishWithNoResult;
- (void)sequenceDidFinish:(id)arg1;
- (void)enqueueSequence:(id)arg1;
- (id)resultName;
- (void)setResultName:(id)arg1;
- (id)evalOpExpression;
- (void)setEvalOpExpression:(id)arg1;
- (id)gdbAdaptor;
- (id)syntheticDataValue;
- (void)setSyntheticDataValue:(id)arg1;
- (id)finishTarget;
- (void)setFinishTarget:(id)arg1 action:(SEL)arg2;
- (void)dealloc;
- (id)initWithDescription:(id)arg1 syntheticDataValue:(id)arg2;

@end

