/*
 *     Generated by class-dump 3.3.4 (64 bit).
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2011 by Steve Nygard.
 */

#import "PBXGDB_FetchStringSequence.h"

@interface PBXGDB_CFNSStringContentFetchSequence : PBXGDB_FetchStringSequence
{
    long long _stringLength;
}

- (void)execute;
- (void)safetyCheckFailed;
- (BOOL)needsEvaluationSafetyCheck;
- (void)_gotMemoryAllocation:(id)arg1;
- (void)_getStringCharacters;
- (void)_gotUniChars:(id)arg1;
- (unsigned long long)unicharBufferSize;
- (unsigned long long)numberOfCharactersForStringBuffer;
- (id)stringBufferAddressValue;

@end

