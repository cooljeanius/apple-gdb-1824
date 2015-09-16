/* *INDENT-OFF* */
/* THIS FILE IS GENERATED */
/* editing manually until the Makefile rule generates it properly */

#include "server.h"
#include "regdef.h"
#include "regcache.h"
#include "tdesc.h"

/* prototypes: */
extern void init_registers_x86_64(void);
extern void init_registers_amd64(void);
extern void init_registers_amd64_avx(void);
extern void init_registers_amd64_avx512(void);
extern void init_registers_amd64_mpx(void);
extern void init_registers_x32(void);
extern void init_registers_x32_avx(void);
extern void init_registers_x32_avx512(void);

#ifndef _INIT_REGISTERS_DECLARED
# define _INIT_REGISTERS_DECLARED 1
void init_registers(void) {
	init_registers_x86_64();
	init_registers_amd64();
	init_registers_amd64_avx();
	init_registers_amd64_avx512();
	init_registers_amd64_mpx();
	init_registers_x32();
	init_registers_x32_avx();
	init_registers_x32_avx512();
}
#endif /* !_INIT_REGISTERS_DECLARED */

