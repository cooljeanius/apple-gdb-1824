/*==============================================================================
 * $Id$
 *
 *! XCode variable formatter for Unicode character data.
 *! See http://www.elegantchaos.com/projects/unicode-formatter for more info.
 *
 * Copyright (c) 2009 Elegant Chaos. Based on Apple sample code.
 *============================================================================*/

/*------------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/

#include "unicode_formatter.h"

#import "/Developer/Applications/Xcode.app/Contents/PlugIns/GDBMIDebugging.xcplugin/Contents/Headers/DataFormatterPlugin.h"

/*------------------------------------------------------------------------------
 * Globals
 *----------------------------------------------------------------------------*/

/* A bundle to support custom data formatting must define this symbol: */
_pbxgdb_plugin_function_list *_pbxgdb_plugin_functions;

/*------------------------------------------------------------------------------
 * Constants
 *----------------------------------------------------------------------------*/

char* kNullPluginError = "CFDataFormatter plugin error: _pbxgdb_plugin_functions not set!";
char* kNullInputError = "<null string>";
const size_t max_length = 1024;

/*------------------------------------------------------------------------------
 *! Convert a CFString into a char* buffer that we can return.
 *----------------------------------------------------------------------------*/

char* ConvertStringToEncoding(CFStringRef string, CFStringEncoding encoding, int identifier);
char* ConvertStringToEncoding(CFStringRef string, CFStringEncoding encoding, int identifier)
{
	char* output = NULL;
    if (_pbxgdb_plugin_functions)
	{
		/* we want to convert the whole string */
		CFRange range;
		range.location = 0;
		range.length = CFStringGetLength(string);

		/* find out how big our utf-8 buffer needs to be */
		CFIndex length_needed;
		CFStringGetBytes(string, range, encoding, '?', false, NULL, 0, &length_needed);

		/* make the output buffer */
		/* just in case the convert call fails completely, we zero terminate it */
        output = (char*)(_pbxgdb_plugin_functions->allocate(identifier, length_needed + 1));
		if (output)
		{
			output[0] = 0;

			/* try to get the actual string - we terminate it for safety */
			CFStringGetBytes(string, range, encoding, '?', false, (UInt8*) output, length_needed, &length_needed);
			output[length_needed] = 0;
		}
    }

	if (output) DEBUG_PRINT("converted: %s\n", output);


	return output ? output : kNullPluginError;
}

/*------------------------------------------------------------------------------
 *! Takes a unicode character and returns it as a C string.
 *----------------------------------------------------------------------------*/

char* formatUnicodeCharacter(long character, int identifier);
char* formatUnicodeCharacter(long character, int identifier)
{
	CFStringRef string = CFStringCreateWithBytes(NULL, (UInt8*) &character, sizeof(character), kCFStringEncodingUTF32LE, false);
	char* result = ConvertStringToEncoding(string, kCFStringEncodingUTF8, identifier);
	CFRelease(string);

	return result;
}

/*------------------------------------------------------------------------------
 *! Takes a pointer to a unicode string, and returns it as a C string.
 *! The size of 1 character is passed in, so that we can tell if we are dealing
 *! with 16 or 32 bit characters.
 *----------------------------------------------------------------------------*/

char* formatUnicodeString(long* input, size_t size_of_one_char, int identifier);
char* formatUnicodeString(long* input, size_t size_of_one_char, int identifier)
{
	if (!input)
		return kNullInputError;

	size_t length = 0;
	CFStringEncoding encoding;

	if (size_of_one_char == 2)
	{
		short* temp = (short*) input;
		while (*temp++ && (length < max_length)) length++;
		encoding = kCFStringEncodingUTF16LE;
	}
	else
	{
		long* temp = (long*) input;
		while (*temp++ && (length < max_length)) length++;
		encoding = kCFStringEncodingUTF32LE;
	}

	CFStringRef string = CFStringCreateWithBytes(NULL, (UInt8*) input, length * size_of_one_char, encoding, false);
	char* result = ConvertStringToEncoding(string, kCFStringEncodingUTF8, identifier);
	CFRelease(string);

	return result;
}

/* EOF */
