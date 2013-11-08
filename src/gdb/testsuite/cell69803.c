#include <libspe2.h>
extern spe_program_handle_t cell69803_spu_x;
int main (void) {
unsigned int entry = SPE_DEFAULT_ENTRY;
spe_context_ptr_t ctx = spe_context_create (0, NULL);
spe_program_load (ctx, &cell69803_spu_x);
return spe_context_run (ctx, &entry, 0, NULL, NULL, NULL); }
