/*=============================================================================
 * $Id: unicode_formatter.c 846 2009-06-10 23:41:47Z sam $
 *
 *! Specific formatters for some custom library types. Not a great deal of
 *! use for anyone in the outside world.
 *============================================================================*/

/*------------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/

#include "unicode_formatter.h"

/*------------------------------------------------------------------------------
 *! Takes a pointer to a si core string
 *----------------------------------------------------------------------------*/

char* formatCoreString(long* input, int identifier);
char* formatCoreString(long* input, int identifier)
{
	if (!input)
		return kNullInputError;

	if (((char*) input)[0] != '\1')
		return (char*) input;

	size_t length = 0;
	short* actual_buffer = (short*) (input + 1);
	short* temp = actual_buffer;
	while (*temp++ && (length < max_length)) length++;

	CFStringRef string = CFStringCreateWithBytes(NULL, (UInt8*) actual_buffer, length * 2, kCFStringEncodingUTF16LE, false);
	char* result = ConvertStringToEncoding(string, kCFStringEncodingUTF8, identifier);
	CFRelease(string);

	return result;
}

/* EOF */
