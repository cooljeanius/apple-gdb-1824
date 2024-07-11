/* Support for printing Chill types for GDB, the GNU debugger.
   Copyright 1986, 1988, 1989, 1991 Free Software Foundation, Inc.

This file is part of GDB.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#include "defs.h"
#include "obstack.h"
#include "bfd.h"		/* Binary File Description */
#include "symtab.h"
#include "gdbtypes.h"
#include "expression.h"
#include "value.h"
#include "gdbcore.h"
#include "target.h"
#include "command.h"
#include "gdbcmd.h"
#include "language.h"
#include "demangle.h"
#include "ch-lang.h"
#include "typeprint.h"

#include "gdb_string.h"
#include <errno.h>

static void
chill_type_print_base PARAMS((struct type *, struct gdb_file *, int, int));

void
chill_print_type(struct type *type, const char *varstring,
		 struct gdb_file *stream, int show, int level)
{
  if (varstring != NULL && *varstring != '\0')
    {
      fputs_filtered(varstring, (struct ui_file *)stream);
      fputs_filtered(" ", (struct ui_file *)stream);
    }
  chill_type_print_base(type, stream, show, level);
}

/* Print the name of the type (or the ultimate pointer target,
   function value or array element).

   SHOW nonzero means don't print this type as just its name;
   show its real definition even if it has a name.
   SHOW zero means print just typename or tag if there is one
   SHOW negative means abbreviate structure elements.
   SHOW is decremented for printing of structure elements.

   LEVEL is the depth to indent by.
   We increase it for some recursive calls.  */
static void
chill_type_print_base(struct type *type, struct gdb_file *stream, int show,
		      int level)
{
  register int len;
  register int i;
  struct type *index_type;
  struct type *range_type;
  LONGEST low_bound;
  LONGEST high_bound;

  QUIT;

  wrap_here("    ");
  if (type == NULL)
    {
      fputs_filtered("<type unknown>", (struct ui_file *)stream);
      return;
    }

  /* When SHOW is zero or less, and there is a valid type name, then always
     just print the type name directly from the type. */

  if ((show <= 0) && (TYPE_NAME(type) != NULL))
    {
      fputs_filtered(TYPE_NAME(type), (struct ui_file *)stream);
      return;
    }

  if (TYPE_CODE(type) != TYPE_CODE_TYPEDEF)
    CHECK_TYPEDEF(type);

  switch (TYPE_CODE(type))
    {
      case TYPE_CODE_TYPEDEF:
        chill_type_print_base(TYPE_TARGET_TYPE(type), stream, 0, level);
	break;
      case TYPE_CODE_PTR:
	if (TYPE_CODE(TYPE_TARGET_TYPE(type)) == TYPE_CODE_VOID)
	  {
	    fprintf_filtered((struct ui_file *)stream, "%s",
			     TYPE_NAME(type) ? TYPE_NAME(type) : "PTR");
	    break;
	  }
	fprintf_filtered((struct ui_file *)stream, "REF ");
	chill_type_print_base(TYPE_TARGET_TYPE(type), stream, 0, level);
	break;

      case TYPE_CODE_BOOL:
	/* FIXME: we should probably just print the TYPE_NAME, in case
	   anyone ever fixes the compiler to give us the real names
	   in the presence of the chill equivalent of typedef (assuming
	   there is one).  */
	fprintf_filtered((struct ui_file *)stream, "%s",
			 TYPE_NAME(type) ? TYPE_NAME(type) : "BOOL");
	break;

      case TYPE_CODE_ARRAY:
        fputs_filtered("ARRAY (", (struct ui_file *)stream);
	range_type = TYPE_FIELD_TYPE(type, 0);
	if (TYPE_CODE(range_type) != TYPE_CODE_RANGE)
	  chill_print_type(range_type, "", stream, 0, level);
	else
	  {
	    index_type = TYPE_TARGET_TYPE(range_type);
	    low_bound = TYPE_FIELD_BITPOS(range_type, 0);
	    high_bound = TYPE_FIELD_BITPOS(range_type, 1);
	    print_type_scalar(index_type, low_bound, (struct ui_file *)stream);
	    fputs_filtered(":", (struct ui_file *)stream);
	    print_type_scalar(index_type, high_bound, (struct ui_file *)stream);
	  }
	fputs_filtered(") ", (struct ui_file *)stream);
	chill_print_type(TYPE_TARGET_TYPE(type), "", stream, 0, level);
	break;

      case TYPE_CODE_BITSTRING:
        fprintf_filtered((struct ui_file *)stream, "BOOLS (%d)",
			 TYPE_FIELD_BITPOS(TYPE_FIELD_TYPE(type, 0), 1) + 1);
	break;

      case TYPE_CODE_SET:
        fputs_filtered("POWERSET ", (struct ui_file *)stream);
	chill_print_type(TYPE_INDEX_TYPE(type), "", stream,
			 (show - 1), level);
	break;

      case TYPE_CODE_STRING:
	range_type = TYPE_FIELD_TYPE(type, 0);
	index_type = TYPE_TARGET_TYPE(range_type);
	high_bound = TYPE_FIELD_BITPOS(range_type, 1);
        fputs_filtered("CHARS (", (struct ui_file *)stream);
	print_type_scalar(index_type, (high_bound + 1),
			  (struct ui_file *)stream);
	fputs_filtered(")", (struct ui_file *)stream);
	break;

      case TYPE_CODE_MEMBER:
	fprintf_filtered((struct ui_file *)stream, "MEMBER ");
        chill_type_print_base(TYPE_TARGET_TYPE(type), stream, 0, level);
	break;
      case TYPE_CODE_REF:
	fprintf_filtered((struct ui_file *)stream, "/*LOC*/ ");
        chill_type_print_base(TYPE_TARGET_TYPE(type), stream, show, level);
	break;
      case TYPE_CODE_FUNC:
	fprintf_filtered((struct ui_file *)stream, "PROC (");
	len = TYPE_NFIELDS(type);
	for (i = 0; i < len; i++)
	  {
	    struct type *param_type = TYPE_FIELD_TYPE(type, i);
	    if (i > 0)
	      {
		fputs_filtered(", ", (struct ui_file *)stream);
		wrap_here("    ");
	      }
	    if (TYPE_CODE(param_type) == TYPE_CODE_REF)
	      {
		chill_type_print_base(TYPE_TARGET_TYPE (param_type),
                                      stream, 0, level);
		fputs_filtered(" LOC", (struct ui_file *)stream);
	      }
	    else
	      chill_type_print_base(param_type, stream, show, level);
	  }
	fprintf_filtered((struct ui_file *)stream, ")");
	if (TYPE_CODE(TYPE_TARGET_TYPE(type)) != TYPE_CODE_VOID)
	  {
	    fputs_filtered(" RETURNS (", (struct ui_file *)stream);
	    chill_type_print_base(TYPE_TARGET_TYPE(type), stream, 0, level);
	    fputs_filtered(")", (struct ui_file *)stream);
	  }
	break;

      case TYPE_CODE_STRUCT:
	if (chill_varying_type(type))
	  {
	    chill_type_print_base(TYPE_FIELD_TYPE (type, 1),
				  stream, 0, level);
	    fputs_filtered(" VARYING", (struct ui_file *)stream);
	  }
	else
	  {
	    fprintf_filtered((struct ui_file *)stream, "STRUCT ");

	    fprintf_filtered((struct ui_file *)stream, "(\n");
	    if ((TYPE_NFIELDS(type) == 0) && (TYPE_NFN_FIELDS(type) == 0))
	      {
		if (TYPE_FLAGS(type) & TYPE_FLAG_STUB)
		  {
		    fprintfi_filtered((level + 4), (struct ui_file *)stream,
                                      "<incomplete type>\n");
		  }
		else
		  {
		    fprintfi_filtered((level + 4), (struct ui_file *)stream,
                                      "<no data fields>\n");
		  }
	      }
	    else
	      {
		len = TYPE_NFIELDS(type);
		for (i = TYPE_N_BASECLASSES(type); i < len; i++)
		  {
		    struct type *field_type = TYPE_FIELD_TYPE(type, i);
		    QUIT;
		    print_spaces_filtered((level + 4),
                                          (struct ui_file *)stream);
		    if (TYPE_CODE(field_type) == TYPE_CODE_UNION)
		      { int j; /* variant number */
			fputs_filtered("CASE OF\n", (struct ui_file *)stream);
			for (j = 0; j < TYPE_NFIELDS(field_type); j++)
			  { int k; /* variant field index */
			    struct type *variant_type =
                              TYPE_FIELD_TYPE(field_type, j);
			    int var_len = TYPE_NFIELDS(variant_type);
			    print_spaces_filtered((level + 4),
                                                  (struct ui_file *)stream);
			    if (strcmp(TYPE_FIELD_NAME(field_type, j),
                                       "else") == 0)
			      fputs_filtered("ELSE\n",
                                             (struct ui_file *)stream);
			    else
			      fputs_filtered(":\n", (struct ui_file *)stream);
			    if (TYPE_CODE(variant_type) != TYPE_CODE_STRUCT)
			      error("variant record confusion");
			    for (k = 0; k < var_len; k++)
			      {
				print_spaces_filtered((level + 8),
                                                      (struct ui_file *)stream);
				chill_print_type(TYPE_FIELD_TYPE(variant_type, k),
						 TYPE_FIELD_NAME(variant_type, k),
						 stream, (show - 1),
             					 (level + 8));
				if (k < (var_len - 1))
				  fputs_filtered(",", (struct ui_file *)stream);
				fputs_filtered("\n", (struct ui_file *)stream);
			      }
			  }
			print_spaces_filtered((level + 4),
                                              (struct ui_file *)stream);
			fputs_filtered("ESAC", (struct ui_file *)stream);
		      }
		    else
		      chill_print_type(field_type,
                                       TYPE_FIELD_NAME(type, i),
                                       stream, (show - 1), (level + 4));
		    if (i < (len - 1))
		      {
			fputs_filtered(",", (struct ui_file *)stream);
		      }
		    fputs_filtered("\n", (struct ui_file *)stream);
		  }
	      }
	    fprintfi_filtered(level, (struct ui_file *)stream, ")");
	  }
	break;

      case TYPE_CODE_RANGE:
	  {
	    struct type *target = TYPE_TARGET_TYPE(type);
	    if (target && TYPE_NAME(target))
	      fputs_filtered(TYPE_NAME(target), (struct ui_file *)stream);
	    else
	      fputs_filtered("RANGE", (struct ui_file *)stream);
	    if (target == NULL)
	      target = builtin_type_long;
	    fputs_filtered(" (", (struct ui_file *)stream);
	    print_type_scalar(target, TYPE_LOW_BOUND(type),
                              (struct ui_file *)stream);
	    fputs_filtered(":", (struct ui_file *)stream);
	    print_type_scalar(target, TYPE_HIGH_BOUND(type),
                              (struct ui_file *)stream);
	    fputs_filtered(")", (struct ui_file *)stream);
	  }
	break;

      case TYPE_CODE_ENUM:
	{
	  register int lastval = 0;
	  fprintf_filtered((struct ui_file *)stream, "SET (");
	  len = TYPE_NFIELDS(type);
	  for (i = 0; i < len; i++)
	    {
	      QUIT;
	      if (i) fprintf_filtered((struct ui_file *)stream, ", ");
	      wrap_here("    ");
	      fputs_filtered(TYPE_FIELD_NAME(type, i),
                             (struct ui_file *)stream);
	      if (lastval != TYPE_FIELD_BITPOS(type, i))
		{
		  fprintf_filtered((struct ui_file *)stream, " = %d",
                                   TYPE_FIELD_BITPOS(type, i));
		  lastval = TYPE_FIELD_BITPOS(type, i);
		}
	      lastval++;
	    }
	  fprintf_filtered((struct ui_file *)stream, ")");
	}
	break;

      case TYPE_CODE_VOID:
      case TYPE_CODE_UNDEF:
      case TYPE_CODE_ERROR:
      case TYPE_CODE_UNION:
      case TYPE_CODE_METHOD:
	error("missing language support in chill_type_print_base");
	break;

      default:

	/* Handle types not explicitly handled by the other cases,
	   such as fundamental types.  For these, just print whatever
	   the type name is, as recorded in the type itself.  If there
	   is no type name, then complain. */
	if (TYPE_NAME(type) != NULL)
	  {
	    fputs_filtered(TYPE_NAME(type), (struct ui_file *)stream);
	  }
	else
	  {
	    error("Unrecognized type code (%d) in symbol table.",
		  TYPE_CODE(type));
	  }
	break;
      }
}
