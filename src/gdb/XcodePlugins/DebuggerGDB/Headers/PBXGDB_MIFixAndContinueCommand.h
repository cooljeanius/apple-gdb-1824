/*
 *     Generated by class-dump 3.3.4 (64 bit).
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2011 by Steve Nygard.
 */

#import <DebuggerGDB/PBXGDB_MICommand.h>

@class NSString;

@interface PBXGDB_MIFixAndContinueCommand : PBXGDB_MICommand
{
    NSString *_pathToFnCPatchFile;
    NSString *_pathToSourceFile;
    NSString *_pathToObjectFile;
    NSString *_shlib;
}

- (id)GDBCommandString;
- (void)setFnCPatchFilePath:(id)arg1 sourceFilePath:(id)arg2 objectFilePath:(id)arg3 shlib:(id)arg4;
- (void)setShlib:(id)arg1;
- (void)setPathOfFnCPatchFile:(id)arg1;
- (void)setPathOfSourceFile:(id)arg1;
- (void)setPathOfObjectFile:(id)arg1;

@end
