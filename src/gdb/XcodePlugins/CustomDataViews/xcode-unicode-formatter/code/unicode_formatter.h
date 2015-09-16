// ================================================================================
// $Id: unicode_formatter.c 846 2009-06-10 23:41:47Z sam $
//
//! XCode variable formatter for Unicode character data.
//! See http://www.elegantchaos.com/projects/unicode-formatter for more info.
//
// Copyright (c) 2009 Elegant Chaos. Based on Apple sample code.
// ================================================================================

// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------

#include <CoreFoundation/CoreFoundation.h>

// --------------------------------------------------------------------------------
// Macros
// --------------------------------------------------------------------------------

#define DEBUG_PRINT(...)
//#define DEBUG_PRINT(...) fprintf(stderr, __VA_ARGS__);

// --------------------------------------------------------------------------------
// Constants
// --------------------------------------------------------------------------------

extern char* kNullPluginError;
extern char* kNullInputError;
extern const size_t max_length;

// --------------------------------------------------------------------------------
//! Convert a CFString into a char* buffer that we can return.
// --------------------------------------------------------------------------------

char* ConvertStringToEncoding(CFStringRef string, CFStringEncoding encoding, int identifier);
