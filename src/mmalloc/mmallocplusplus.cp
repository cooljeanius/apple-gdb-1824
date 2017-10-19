/* mmalloc.cp -*- C++ -*- */

#include "config.h"
#include <cstring>
#include <iostream>

#ifndef __cplusplus
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This file should be compiled as C++."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* !__cplusplus */

/* specifically _avoid_ any 'extern "C"' statements here; in the hopes
 * that name mangling will prevent any would-be duplicate symbols from
 * being multiply defined... */

#include "mmalloc.c"

/* EOF */
