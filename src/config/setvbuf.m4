#setvbuf.m4 serial 1
# taken from autoconf2.61
# obsolete in newer versions

# AC_FUNC_SETVBUF_REVERSED
# ------------------------
AN_FUNCTION([setvbuf],[AC_FUNC_SETVBUF_REVERSED])dnl
AC_DEFUN([AC_FUNC_SETVBUF_REVERSED],
[AC_REQUIRE([AC_C_PROTOTYPES])dnl
AC_CACHE_CHECK([whether setvbuf arguments are reversed],
  [ac_cv_func_setvbuf_reversed],
  [ac_cv_func_setvbuf_reversed=no
   AC_LINK_IFELSE(
     [AC_LANG_PROGRAM(
	[[#include <stdio.h>
#	  ifdef PROTOTYPES
	   int (setvbuf)(FILE *, int, char *, size_t);
#	  endif /* PROTOTYPES */]],
	[[char buf; return setvbuf(stdout, _IOLBF, &buf, 1);]])],
     [AC_LINK_IFELSE(
	[AC_LANG_PROGRAM(
	   [[#include <stdio.h>
#	     ifdef PROTOTYPES
	      int (setvbuf)(FILE *, int, char *, size_t);
#	     endif /* PROTOTYPES */]],
	   [[char buf; return setvbuf(stdout, &buf, _IOLBF, 1);]])],
	[# It compiles and links either way, so it must not be declared
	 # with a prototype and most likely this is a K&R C compiler.
	 # Try running it.
	 AC_RUN_IFELSE(
	   [AC_LANG_PROGRAM(
	      [AC_INCLUDES_DEFAULT],
	      [[/* This call has the arguments reversed.
		 * A reversed system may check & see that the addr of buf
		 * is not _IOLBF, _IONBF, or _IOFBF, & return nonzero. */
		char buf;
		if (setvbuf(stdout, _IOLBF, &buf, 1) != 0) {
		  return 1;
		}
		putchar('\r');
		return 0; /* Non-reversed systems SEGV here. */]])],
	   [ac_cv_func_setvbuf_reversed=yes],
	   [],
	   [[: # Assume setvbuf is not reversed when cross-compiling.]])]
	ac_cv_func_setvbuf_reversed=yes)])])
if test "x${ac_cv_func_setvbuf_reversed}" = "xyes"; then
  AC_DEFINE([SETVBUF_REVERSED],[1],
	    [Define to 1 if the `setvbuf' func takes the buffering type as
	     its second argument and the buffer pointer as the third, as on
	     System V before release 3.])
fi
])dnl# AC_FUNC_SETVBUF_REVERSED


# AU::AC_SETVBUF_REVERSED
# -----------------------
AU_ALIAS([AC_SETVBUF_REVERSED],[AC_FUNC_SETVBUF_REVERSED])dnl
