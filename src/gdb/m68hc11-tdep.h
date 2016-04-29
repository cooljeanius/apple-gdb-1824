/* m68hc11-tdep.h */

#ifndef M68HC11_TDEP_H
#define M68HC11_TDEP_H 1

struct m68hc11_unwind_cache *m68hc11_frame_unwind_cache(struct frame_info *,
							void **);
extern const struct frame_unwind *m68hc11_frame_sniffer(struct frame_info *);
extern enum return_value_convention m68hc11_return_value(struct gdbarch *,
							 struct type *,
							 struct regcache *,
							 gdb_byte *,
							 const gdb_byte *);

#endif /* !M68HC11_TDEP_H */

/* EOF */
