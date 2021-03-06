/*
 *     Generated by class-dump 3.3.4 (64 bit).
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2011 by Steve Nygard.
 */

#import "NSObject.h"

@class NSMutableArray, NSString, PBXGDB_MIController, PBXGDB_MIResultList;

@interface PBXGDB_MICommand : NSObject
{
    unsigned long long _sequenceNumber;
    id _target;
    SEL _finishAction;
    SEL _startExecutableRunningAction;
    NSString *_commandString;
    NSMutableArray *_resultDescriptions;
    PBXGDB_MIController *_miController;
    long long _threadIdContext;
    long long _frameIndexContext;
    PBXGDB_MIResultList *_parseResults;
    NSString *_errorMessage;
}

+ (id)miCommand;
+ (unsigned long long)nextSequenceNumber;
- (void)gdbStoppedWithResults:(id)arg1;
- (id)errorMessage;
- (void)setErrorMessage:(id)arg1;
- (BOOL)needsUnwindOnSignal;
- (id)resultDescriptions;
- (void)allocateResultDescription;
- (id)miController;
- (id)GDBCommandString;
- (void)setGDBCommandString:(id)arg1;
- (id)rawCommandString;
- (void)didStartExecutableRunning;
- (id)parseResults;
- (void)setParseResults:(id)arg1;
- (void)didFinishWithResultList:(id)arg1;
- (SEL)startExecutableRunningAction;
- (void)setStartExecutableRunningAction:(SEL)arg1;
- (SEL)finishAction;
- (void)setFinishAction:(SEL)arg1;
- (id)target;
- (void)setTarget:(id)arg1;
- (long long)frameIndexContext;
- (void)setFrameIndexContext:(long long)arg1;
- (long long)threadIdContext;
- (void)setThreadIdContext:(long long)arg1;
- (BOOL)waitsForOutput;
- (id)GDBCommandStringForExecution;
- (void)willExecute;
- (void)setMIController:(id)arg1;
- (unsigned long long)sequenceNumber;
- (void)dealloc;
- (id)initWithSequenceNumber:(unsigned long long)arg1;

@end

