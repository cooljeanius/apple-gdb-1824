/*
 *     Generated by class-dump 3.3.4 (64 bit).
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2011 by Steve Nygard.
 */

#import <DebuggerGDB/PBXGDB_SyntheticDataValue.h>

@class NSString;

@interface PBXGDB_VarObjDataValue2 : PBXGDB_SyntheticDataValue
{
    int _typeCode;
    NSString *_varObjName;
}

+ (BOOL)isNSArrayType:(id)arg1;
+ (BOOL)isSTLVectorType:(id)arg1;
+ (BOOL)isArrayType:(int)arg1 name:(id)arg2 type:(id)arg3;
+ (void)initialize;
- (void)requestGetPathExpressionString;
- (BOOL)canGetPathExpressionString;
- (id)summary;
- (void)setStringValue:(id)arg1;
- (BOOL)isFourCharCodeType;
- (id)_pointerValueStringForString:(id)arg1;
- (id)_boolValueStringForString:(id)arg1;
- (void)requestSetStringValue:(id)arg1;
- (id)_booleanValueSetStringForValueString:(id)arg1;
- (BOOL)valueIsEditable;
- (void)requestInfoReferencesInThread:(id)arg1 atStackFrame:(id)arg2;
- (void)requestInfoRootsInThread:(id)arg1 atStackFrame:(id)arg2;
- (void)requestPrintDescriptionInThread:(id)arg1 atStackFrame:(id)arg2 runAllThreads:(BOOL)arg3;
- (BOOL)canPrintGCInformation;
- (BOOL)canPrintDescription;
- (BOOL)isNSArrayType;
- (BOOL)isSTLVectorType;
- (BOOL)isArrayType;
- (BOOL)isPointerType;
- (int)typeCode;
- (void)setTypeCode:(int)arg1;
- (id)varObjName;
- (id)varObjNameArrayWithChildren;
- (void)setVarObjName:(id)arg1;
- (void)requestSetValueFormat:(int)arg1;
- (BOOL)canSetValueFormat;
- (void)requestReleaseChildrenFreeVarObjs:(BOOL)arg1;
- (void)requestReleaseChildren;
- (void)requestGetChildren;
- (void)deleteVarObjFromGDB;
- (id)contentDescription;
- (void)dealloc;

@end

