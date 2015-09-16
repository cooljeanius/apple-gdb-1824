/************************************************************

	Assert.h
	Diagnostics

	Copyright (c) Apple Computer,Inc.  1987-1990.
	All Rights Reserved.

************************************************************/

#undef assert

#ifdef NDEBUG

# define assert(ignore) ((void) 0)

#else

# ifdef __cplusplus
extern "C" {
# endif /* __cplusplus */
void __assertprint(const char* file, int line, const char* expr);
# ifdef __cplusplus
}
# endif /* __cplusplus */

# define assert(expression) \
	( (expression) ? (void) 0 : (__assertprint(__FILE__, __LINE__, #expression)) )

#endif /* NDEBUG */

/* EOF */
