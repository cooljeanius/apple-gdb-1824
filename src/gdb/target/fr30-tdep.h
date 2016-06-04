/* fr30-tdep.h */

#ifndef FR30_TDEP_H
#define FR30_TDEP_H 1

#ifndef TM_FR30_H
# include "config/fr30/tm-fr30.h"
#endif /* !TM_FR30_H */

extern CORE_ADDR fr30_find_callers_reg(struct frame_info *, int);
extern CORE_ADDR fr30_frame_chain(struct frame_info *);
extern CORE_ADDR fr30_frame_saved_pc(struct frame_info *);
extern int fr30_fix_call_dummy(char *, CORE_ADDR, CORE_ADDR, int,
			       struct value **, struct type *, int);

#endif /* !FR30_TDEP_H */

/* EOF */
