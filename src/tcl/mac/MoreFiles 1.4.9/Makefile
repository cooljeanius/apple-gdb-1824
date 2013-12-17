#--------------------------------------------------------------------------
#
#	Apple Macintosh Developer Technical Support
#
#	Program is...	MoreFilesLib
#	File is...		MoreFilesLib.make
#
#	Copyright (c) 1996-1998 Apple Computer, Inc.
#	All rights reserved.
#
#--------------------------------------------------------------------------

#	The three possible targets for this makefile
#
#	The default name is LibName and builds the 68K library
#	named MoreFilesLib.o
#
#	PowerPC library is named MoreFilesLib.PPC and is built with:
#		make -f MoreFilesLib.make MoreFilesLib.PPC
#
#	The "Fat" version is NOT actually fat, it just builds both 68K and 
#		PowerPC versions.  Build it with:
#		make -f MoreFilesLib.make MoreFilesLib.FAT
#
LibName			=	MoreFilesLib.o
LibNamePPC		=	MoreFilesLib.PPC
LibNameFAT		=	MoreFilesLib.FAT

MoreFiles.hdrs		=	"C Headers"
MPW.hdrs		=	../../../include/mpw/CIncludes
projsrc			=	Sources
obj			= 	Objects
objppc			= 	ObjectsPPC
libs			= 	Libs

DebugTarget		=	#(debug)
MacsBugSyms		=	-g

# SymOptions and OptOptions are mutually exclusive. Enable as appropriate.
SymOptions		=	-ggdb 
# turn the above on to debug with SADE/R2Db
COptOptions		=	-O
# turn the above off to build debug 68K code
MrCOptOptions		=	-Os
# turn the above off to build debug PowerPC code

IncludesFolders		=	-I$(MoreFiles.hdrs) -I$(MPW.hdrs)
COptions		=	$(IncludesFolders) $(SymOptions) $(COptOptions) $(MacsBugSyms)
MrCOptions		=	$(IncludesFolders) $(SymOptions) $(MrCOptOptions)
LibOptions		=	$(SymOptions)
LibOptionsPPC		=	$(SymOptions)

#--------------------------------------------------------------------------
# These are modified default build rules, which allow us to build using
# both SC (68K) and MrC (PowerPC) from the same set of sources
#--------------------------------------------------------------------------
all: $(LibNameFAT)

$(obj): $(projsrc)

.c.o: %.c
	@echo "# compiling "$<" using "$(CC)
	$(CC) $(COptions) $(CAltOptions) -o $@ $<

$(obj)/%.c.o: $(projsrc)/%.c
	@echo "# compiling "$<" using "$(CC)
	$(CC) $(COptions) $(CAltOptions) -o $@ $<

$(objppc): $(projsrc)

.c.x: %.c
	@echo "# compiling "$<" using MrC"
	$(CC) -arch ppc $(MrCOptions) -o $@ $<

$(objppc)/%.c.x: $(projsrc)/%.c
	@echo "# compiling "$<" using MrC"
	$(CC) -arch ppc $(MrCOptions) -o $@ $<

#--------------------------------------------------------------------------
# These are the objects that we want to include in the library.
#--------------------------------------------------------------------------
LibObjects		=	\
					$(obj)/DirectoryCopy.c.o \
					$(obj)/FileCopy.c.o \
					$(obj)/FSpCompat.c.o \
					$(obj)/FullPath.c.o \
					$(obj)/IterateDirectory.c.o \
					$(obj)/MoreDesktopMgr.c.o \
					$(obj)/MoreFiles.c.o \
					$(obj)/MoreFilesExtras.c.o \
					$(obj)/Search.c.o

LibObjectsPPC	=	\
					$(objppc)/DirectoryCopy.c.x \
					$(objppc)/FileCopy.c.x \
					$(objppc)/FSpCompat.c.x \
					$(objppc)/FullPath.c.x \
					$(objppc)/IterateDirectory.c.x \
					$(objppc)/MoreDesktopMgr.c.x \
					$(objppc)/MoreFiles.c.x \
					$(objppc)/MoreFilesExtras.c.x \
					$(objppc)/Search.c.x

$(LibNameFAT): $(LibName) $(LibNamePPC)

$(LibName):: $(LibObjects)
	@echo "# building 68k library"
	$(LD) $(LibObjects) $(LibOptions) -o "$(libs)/$(LibName)"

$(LibNamePPC):: $(LibObjectsPPC)
	@echo "# building PowerPC library"
	$(LD) -arch ppc $(LibObjectsPPC) $(LibOptionsPPC) -o "$(libs)/$(LibNamePPC)"

