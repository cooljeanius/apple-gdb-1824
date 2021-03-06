/*
 *     Generated by class-dump 3.3.4 (64 bit).
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2011 by Steve Nygard.
 */

#import "NSObject.h"

@class NSArray, NSMutableArray, PBXLSDebuggerAdaptor;

@interface PBXLSModel : NSObject
{
    PBXLSDebuggerAdaptor *_debugger;
    PBXLSModel *_parentModel;
    int _validityStatus;
    NSMutableArray *_children;
    int _childValidityStatus;
}

+ (id)stringForValidityStatus:(int)arg1;
+ (void)initialize;
@property int childValidityStatus; // @synthesize childValidityStatus=_childValidityStatus;
@property int validityStatus; // @synthesize validityStatus=_validityStatus;
@property(retain) PBXLSDebuggerAdaptor *debugger; // @synthesize debugger=_debugger;
- (void)replaceChildAtIndex:(unsigned long long)arg1 withChild:(id)arg2 withKVO:(BOOL)arg3;
- (void)replaceChildAtIndex:(unsigned long long)arg1 withChild:(id)arg2;
- (void)removeChildrenInRange:(struct _NSRange)arg1 withKVO:(BOOL)arg2;
- (void)removeChildrenInRange:(struct _NSRange)arg1;
- (void)removeAllChildren;
- (void)removeChildAtIndex:(unsigned long long)arg1;
- (void)removeChild:(id)arg1;
- (void)insertChild:(id)arg1 atIndex:(unsigned long long)arg2;
- (void)addChildren:(id)arg1;
- (void)addChild:(id)arg1;
- (id)parentModel;
- (void)setParentModel:(id)arg1;
- (id)description;
- (id)contentDescriptionWithChildrenAtLevel:(unsigned long long)arg1;
- (id)contentDescription;
- (id)_paddingForLevel:(unsigned long long)arg1;
- (id)initWithDebugger:(id)arg1 parentModel:(id)arg2;
- (id)initWithDebugger:(id)arg1;

// Remaining properties
@property(readonly) NSArray *children; // @dynamic children;
@property(readonly) NSMutableArray *mutableChildren; // @dynamic mutableChildren;

@end

