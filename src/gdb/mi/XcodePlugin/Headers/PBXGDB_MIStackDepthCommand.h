/*
 *     Generated by class-dump 3.3.4 (64 bit).
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2011 by Steve Nygard.
 */

#import "PBXGDB_MICommand.h"

@interface PBXGDB_MIStackDepthCommand : PBXGDB_MICommand
{
    long long _depth;
}

- (long long)depth;
- (void)didFinishWithResultList:(id)arg1;
- (id)GDBCommandString;

@end
