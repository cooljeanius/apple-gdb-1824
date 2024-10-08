/* stap-probe.c: SystemTap probe support for GDB.

   Copyright (C) 2012-2013 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#if !defined(NO_POISON) && defined(POISON_FREE_TOO)
# define NO_POISON 1 /* (use of free) */
#endif /* !NO_POISON && POISON_FREE_TOO */

#include "defs.h"
#include "stap-probe.h"
#include "probe.h"
#include "vec.h"
#include "ui-out.h"
#include "objfiles.h"
#include "arch-utils.h"
#include "command.h"
#include "gdbcmd.h"
#include "filenames.h"
#include "value.h"
#include "exceptions.h"
#include "ax.h"
#include "ax-gdb.h"
#include "complaints.h"
#include "cli/cli-utils.h"
#include "linespec.h"
#include "user-regs.h"
#include "parser-defs.h"
#include "language.h"
#include "elf-bfd.h"

#include <ctype.h>

/* The name of the SystemTap section where we will find information about
   the probes.  */

#define STAP_BASE_SECTION_NAME ".stapsdt.base"

/* Forward declaration. */

static const struct probe_ops stap_probe_ops;

/* Should we display debug information for the probe's argument expression
   parsing?  */

static unsigned int stap_expression_debug = 0;

/* The various possibilities of bitness defined for a probe's argument.

   The relationship is:

   - STAP_ARG_BITNESS_UNDEFINED:  The user hasn't specified the bitness.
   - STAP_ARG_BITNESS_32BIT_UNSIGNED:  argument string starts with `4@'.
   - STAP_ARG_BITNESS_32BIT_SIGNED:  argument string starts with `-4@'.
   - STAP_ARG_BITNESS_64BIT_UNSIGNED:  argument string starts with `8@'.
   - STAP_ARG_BITNESS_64BIT_SIGNED:  argument string starts with `-8@'.  */

enum stap_arg_bitness
{
  STAP_ARG_BITNESS_UNDEFINED,
  STAP_ARG_BITNESS_32BIT_UNSIGNED,
  STAP_ARG_BITNESS_32BIT_SIGNED,
  STAP_ARG_BITNESS_64BIT_UNSIGNED,
  STAP_ARG_BITNESS_64BIT_SIGNED,
};

/* The following structure represents a single argument for the probe.  */

struct stap_probe_arg
{
  /* The bitness of this argument.  */
  enum stap_arg_bitness bitness;

  /* The corresponding `struct type *' to the bitness.  */
  struct type *atype;

  /* The argument converted to an internal GDB expression.  */
  struct expression *aexpr;
};

typedef struct stap_probe_arg stap_probe_arg_s;
DEF_VEC_O (stap_probe_arg_s);

struct stap_probe
{
  /* Generic information about the probe.  This shall be the first element
     of this struct, in order to maintain binary compatibility with the
     `struct probe' and be able to fully abstract it.  */
  struct probe p;

  /* If the probe has a semaphore associated, then this is the value of
     it.  */
  CORE_ADDR sem_addr;

  unsigned int args_parsed : 1;
  union
    {
      const char *text;

      /* Information about each argument.  This is an array of `stap_probe_arg',
	 with each entry representing one argument.  */
      VEC (stap_probe_arg_s) *vec;
    }
  args_u;
};

/* When parsing the arguments, we have to establish different precedences
   for the various kinds of asm operators.  This enumeration represents those
   precedences.

   This logic behind this is available at
   <http://sourceware.org/binutils/docs/as/Infix-Ops.html#Infix-Ops>, or using
   the command "info '(as)Infix Ops'".  */

enum stap_operand_prec
{
  /* Lowest precedence, used for non-recognized operands or for the beginning
     of the parsing process.  */
  STAP_OPERAND_PREC_NONE = 0,

  /* Precedence of logical OR.  */
  STAP_OPERAND_PREC_LOGICAL_OR,

  /* Precedence of logical AND.  */
  STAP_OPERAND_PREC_LOGICAL_AND,

  /* Precedence of additive (plus, minus) and comparative (equal, less,
     greater-than, etc) operands.  */
  STAP_OPERAND_PREC_ADD_CMP,

  /* Precedence of bitwise operands (bitwise OR, XOR, bitwise AND,
     logical NOT).  */
  STAP_OPERAND_PREC_BITWISE,

  /* Precedence of multiplicative operands (multiplication, division,
     remainder, left shift and right shift).  */
  STAP_OPERAND_PREC_MUL
};

static void stap_parse_argument_1 (struct stap_parse_info *p, int has_lhs,
				   enum stap_operand_prec prec);

static void stap_parse_argument_conditionally (struct stap_parse_info *p);

/* Returns 1 if *S is an operator, zero otherwise.  */

static int stap_is_operator (const char *op);

static void
show_stapexpressiondebug (struct ui_file *file, int from_tty,
			  struct cmd_list_element *c, const char *value)
{
  fprintf_filtered (file, _("SystemTap Probe expression debugging is %s.\n"),
		    value);
}

/* Returns the operator precedence level of OP, or STAP_OPERAND_PREC_NONE
   if the operator code was not recognized.  */

static enum stap_operand_prec
stap_get_operator_prec (enum exp_opcode op)
{
  switch (op)
    {
    case BINOP_LOGICAL_OR:
      return STAP_OPERAND_PREC_LOGICAL_OR;

    case BINOP_LOGICAL_AND:
      return STAP_OPERAND_PREC_LOGICAL_AND;

    case BINOP_ADD:
    case BINOP_SUB:
    case BINOP_EQUAL:
    case BINOP_NOTEQUAL:
    case BINOP_LESS:
    case BINOP_LEQ:
    case BINOP_GTR:
    case BINOP_GEQ:
      return STAP_OPERAND_PREC_ADD_CMP;

    case BINOP_BITWISE_IOR:
    case BINOP_BITWISE_AND:
    case BINOP_BITWISE_XOR:
    case UNOP_LOGICAL_NOT:
      return STAP_OPERAND_PREC_BITWISE;

    case BINOP_MUL:
    case BINOP_DIV:
    case BINOP_REM:
    case BINOP_LSH:
    case BINOP_RSH:
      return STAP_OPERAND_PREC_MUL;

    default:
      return STAP_OPERAND_PREC_NONE;
    }
}

/* Given S, read the operator in it and fills the OP pointer with its code.
   Return 1 on success, zero if the operator was not recognized.  */

static enum exp_opcode
stap_get_opcode (const char **s)
{
  const char c = **s;
  enum exp_opcode op;

  *s += 1;

  switch (c)
    {
    case '*':
      op = BINOP_MUL;
      break;

    case '/':
      op = BINOP_DIV;
      break;

    case '%':
      op = BINOP_REM;
    break;

    case '<':
      op = BINOP_LESS;
      if (**s == '<')
	{
	  *s += 1;
	  op = BINOP_LSH;
	}
      else if (**s == '=')
	{
	  *s += 1;
	  op = BINOP_LEQ;
	}
      else if (**s == '>')
	{
	  *s += 1;
	  op = BINOP_NOTEQUAL;
	}
    break;

    case '>':
      op = BINOP_GTR;
      if (**s == '>')
	{
	  *s += 1;
	  op = BINOP_RSH;
	}
      else if (**s == '=')
	{
	  *s += 1;
	  op = BINOP_GEQ;
	}
    break;

    case '|':
      op = BINOP_BITWISE_IOR;
      if (**s == '|')
	{
	  *s += 1;
	  op = BINOP_LOGICAL_OR;
	}
    break;

    case '&':
      op = BINOP_BITWISE_AND;
      if (**s == '&')
	{
	  *s += 1;
	  op = BINOP_LOGICAL_AND;
	}
    break;

    case '^':
      op = BINOP_BITWISE_XOR;
      break;

    case '!':
      op = UNOP_LOGICAL_NOT;
      break;

    case '+':
      op = BINOP_ADD;
      break;

    case '-':
      op = BINOP_SUB;
      break;

    case '=':
      gdb_assert (**s == '=');
      op = BINOP_EQUAL;
      break;

    default:
      internal_error (__FILE__, __LINE__,
		      _("Invalid opcode in expression `%s' for SystemTap"
			"probe"), *s);
    }

  return op;
}

/* Given the bitness of the argument, represented by B, return the
   corresponding `struct type *'.  */

static struct type *
stap_get_expected_argument_type (struct gdbarch *gdbarch,
				 enum stap_arg_bitness b)
{
  switch (b)
    {
    case STAP_ARG_BITNESS_UNDEFINED:
      if (gdbarch_addr_bit(gdbarch) == 32)
	return get_builtin_type(gdbarch)->builtin_uint32;
      else
	return get_builtin_type(gdbarch)->builtin_uint64;

    case STAP_ARG_BITNESS_32BIT_SIGNED:
      return get_builtin_type(gdbarch)->builtin_int32;

    case STAP_ARG_BITNESS_32BIT_UNSIGNED:
      return get_builtin_type(gdbarch)->builtin_uint32;

    case STAP_ARG_BITNESS_64BIT_SIGNED:
      return get_builtin_type(gdbarch)->builtin_int64;

    case STAP_ARG_BITNESS_64BIT_UNSIGNED:
      return get_builtin_type(gdbarch)->builtin_uint64;

    default:
      internal_error(__FILE__, __LINE__,
                     _("Undefined bitness for probe."));
      break;
    }
}

/* Function responsible for parsing a register operand according to
   SystemTap parlance.  Assuming:

   RP  = register prefix
   RS  = register suffix
   RIP = register indirection prefix
   RIS = register indirection suffix
   
   Then a register operand can be:
   
   [RIP] [RP] REGISTER [RS] [RIS]

   This function takes care of a register's indirection, displacement and
   direct access.  It also takes into consideration the fact that some
   registers are named differently inside and outside GDB, e.g., PPC's
   general-purpose registers are represented by integers in the assembly
   language (e.g., `15' is the 15th general-purpose register), but inside
   GDB they have a prefix (the letter `r') appended.  */

static void
stap_parse_register_operand (struct stap_parse_info *p)
{
  /* Simple flag to indicate whether we have seen a minus signal before
     certain number.  */
  int got_minus = 0;

  /* Flags to indicate whether this register access is being displaced and/or
     indirected.  */
  int disp_p = 0, indirect_p = 0;
  struct gdbarch *gdbarch = p->gdbarch;

  /* Needed to generate the register name as a part of an expression.  */
  struct stoken str;

  /* Variables used to extract the register name from the probe's
     argument.  */
  const char *start;
  char *regname;
  int len;

  /* Prefixes for the parser.  */
  const char *reg_prefix = gdbarch_stap_register_prefix (gdbarch);
  const char *reg_ind_prefix
    = gdbarch_stap_register_indirection_prefix (gdbarch);
  const char *gdb_reg_prefix = gdbarch_stap_gdb_register_prefix (gdbarch);
  int reg_prefix_len = reg_prefix ? strlen (reg_prefix) : 0;
  int reg_ind_prefix_len = reg_ind_prefix ? strlen (reg_ind_prefix) : 0;
  int gdb_reg_prefix_len = gdb_reg_prefix ? strlen (gdb_reg_prefix) : 0;

  /* Suffixes for the parser.  */
  const char *reg_suffix = gdbarch_stap_register_suffix (gdbarch);
  const char *reg_ind_suffix
    = gdbarch_stap_register_indirection_suffix (gdbarch);
  const char *gdb_reg_suffix = gdbarch_stap_gdb_register_suffix (gdbarch);
  int reg_suffix_len = reg_suffix ? strlen (reg_suffix) : 0;
  int reg_ind_suffix_len = reg_ind_suffix ? strlen (reg_ind_suffix) : 0;
  int gdb_reg_suffix_len = gdb_reg_suffix ? strlen (gdb_reg_suffix) : 0;

  /* Checking for a displacement argument.  */
  if (*p->arg == '+')
    {
      /* If it's a plus sign, we don't need to do anything, just advance the
	 pointer.  */
      ++p->arg;
    }

  if (*p->arg == '-')
    {
      got_minus = 1;
      ++p->arg;
    }

  if (isdigit (*p->arg))
    {
      /* The value of the displacement.  */
      long displacement;
      char *endp;

      disp_p = 1;
      displacement = strtol (p->arg, &endp, 10);
      p->arg = endp;

      /* Generating the expression for the displacement.  */
      write_exp_elt_opcode (OP_LONG);
      write_exp_elt_type(get_builtin_type(gdbarch)->builtin_long);
      write_exp_elt_longcst (displacement);
      write_exp_elt_opcode (OP_LONG);
      if (got_minus)
	write_exp_elt_opcode (UNOP_NEG);
    }

  /* Getting rid of register indirection prefix.  */
  if (reg_ind_prefix
      && strncmp (p->arg, reg_ind_prefix, reg_ind_prefix_len) == 0)
    {
      indirect_p = 1;
      p->arg += reg_ind_prefix_len;
    }

  if (disp_p && !indirect_p)
    error (_("Invalid register displacement syntax on expression `%s'."),
	   p->saved_arg);

  /* Getting rid of register prefix.  */
  if (reg_prefix && strncmp (p->arg, reg_prefix, reg_prefix_len) == 0)
    p->arg += reg_prefix_len;

  /* Now we should have only the register name.  Let's extract it and get
     the associated number.  */
  start = p->arg;

  /* We assume the register name is composed by letters and numbers.  */
  while (isalnum (*p->arg))
    ++p->arg;

  len = p->arg - start;

  regname = alloca (len + gdb_reg_prefix_len + gdb_reg_suffix_len + 1);
  regname[0] = '\0';

  /* We only add the GDB's register prefix/suffix if we are dealing with
     a numeric register.  */
  if (gdb_reg_prefix && isdigit (*start))
    {
      strncpy (regname, gdb_reg_prefix, gdb_reg_prefix_len);
      strncpy (regname + gdb_reg_prefix_len, start, len);

      if (gdb_reg_suffix)
	strncpy (regname + gdb_reg_prefix_len + len,
		 gdb_reg_suffix, gdb_reg_suffix_len);

      len += gdb_reg_prefix_len + gdb_reg_suffix_len;
    }
  else
    strncpy (regname, start, len);

  regname[len] = '\0';

  /* Is this a valid register name?  */
  if (user_reg_map_name_to_regnum (gdbarch, regname, len) == -1)
    error (_("Invalid register name `%s' on expression `%s'."),
	   regname, p->saved_arg);

  write_exp_elt_opcode (OP_REGISTER);
  str.ptr = regname;
  str.length = len;
  write_exp_string (str);
  write_exp_elt_opcode (OP_REGISTER);

  if (indirect_p)
    {
      if (disp_p)
	write_exp_elt_opcode (BINOP_ADD);

      /* Casting to the expected type.  */
      write_exp_elt_opcode (UNOP_CAST);
      write_exp_elt_type (lookup_pointer_type (p->arg_type));
      write_exp_elt_opcode (UNOP_CAST);

      write_exp_elt_opcode (UNOP_IND);
    }

  /* Getting rid of the register name suffix.  */
  if (reg_suffix)
    {
      if (strncmp (p->arg, reg_suffix, reg_suffix_len) != 0)
	error (_("Missing register name suffix `%s' on expression `%s'."),
	       reg_suffix, p->saved_arg);

      p->arg += reg_suffix_len;
    }

  /* Getting rid of the register indirection suffix.  */
  if (indirect_p && reg_ind_suffix)
    {
      if (strncmp (p->arg, reg_ind_suffix, reg_ind_suffix_len) != 0)
	error (_("Missing indirection suffix `%s' on expression `%s'."),
	       reg_ind_suffix, p->saved_arg);

      p->arg += reg_ind_suffix_len;
    }
}

/* This function is responsible for parsing a single operand.

   A single operand can be:

      - an unary operation (e.g., `-5', `~2', or even with subexpressions
        like `-(2 + 1)')
      - a register displacement, which will be treated as a register
        operand (e.g., `-4(%eax)' on x86)
      - a numeric constant, or
      - a register operand (see function `stap_parse_register_operand')

   The function also calls special-handling functions to deal with
   unrecognized operands, allowing arch-specific parsers to be
   created.  */

static void
stap_parse_single_operand (struct stap_parse_info *p)
{
  struct gdbarch *gdbarch = p->gdbarch;

  /* Prefixes for the parser.  */
  const char *const_prefix = gdbarch_stap_integer_prefix (gdbarch);
  const char *reg_prefix = gdbarch_stap_register_prefix (gdbarch);
  const char *reg_ind_prefix
    = gdbarch_stap_register_indirection_prefix (gdbarch);
  int const_prefix_len = const_prefix ? strlen (const_prefix) : 0;
  int reg_prefix_len = reg_prefix ? strlen (reg_prefix) : 0;
  int reg_ind_prefix_len = reg_ind_prefix ? strlen (reg_ind_prefix) : 0;

  /* Suffixes for the parser.  */
  const char *const_suffix = gdbarch_stap_integer_suffix (gdbarch);
  int const_suffix_len = const_suffix ? strlen (const_suffix) : 0;

  /* We first try to parse this token as a "special token".  */
  if (gdbarch_stap_parse_special_token_p (gdbarch))
    {
      int ret = gdbarch_stap_parse_special_token (gdbarch, p);

      if (ret)
	{
	  /* If the return value of the above function is not zero,
	     it means it successfully parsed the special token.

	     If it is NULL, we try to parse it using our method.  */
	  return;
	}
    }

  if (*p->arg == '-' || *p->arg == '~' || *p->arg == '+')
    {
      char c = *p->arg;
      int number;

      /* We use this variable to do a lookahead.  */
      const char *tmp = p->arg;

      ++tmp;

      /* This is an unary operation.  Here is a list of allowed tokens
	 here:

	 - numeric literal;
	 - number (from register displacement)
	 - subexpression (beginning with `(')

	 We handle the register displacement here, and the other cases
	 recursively.  */
      if (p->inside_paren_p)
	tmp = skip_spaces_const (tmp);

      if (isdigit (*tmp))
	{
	  char *endp;

	  number = strtol (tmp, &endp, 10);
	  tmp = endp;
	}

      if (!reg_ind_prefix
	  || strncmp (tmp, reg_ind_prefix, reg_ind_prefix_len) != 0)
	{
	  /* This is not a displacement.  We skip the operator, and deal
	     with it later.  */
	  ++p->arg;
	  stap_parse_argument_conditionally (p);
	  if (c == '-')
	    write_exp_elt_opcode (UNOP_NEG);
	  else if (c == '~')
	    write_exp_elt_opcode (UNOP_COMPLEMENT);
	}
      else
	{
	  /* If we are here, it means it is a displacement.  The only
	     operations allowed here are `-' and `+'.  */
	  if (c == '~')
	    error (_("Invalid operator `%c' for register displacement "
		     "on expression `%s'."), c, p->saved_arg);

	  stap_parse_register_operand (p);
	}
    }
  else if (isdigit (*p->arg))
    {
      /* A temporary variable, needed for lookahead.  */
      const char *tmp = p->arg;
      char *endp;
      long number;

      /* We can be dealing with a numeric constant (if `const_prefix' is
	 NULL), or with a register displacement.  */
      number = strtol (tmp, &endp, 10);
      tmp = endp;

      if (p->inside_paren_p)
	tmp = skip_spaces_const (tmp);
      if (!const_prefix && reg_ind_prefix
	  && strncmp (tmp, reg_ind_prefix, reg_ind_prefix_len) != 0)
	{
	  /* We are dealing with a numeric constant.  */
	  write_exp_elt_opcode (OP_LONG);
	  write_exp_elt_type(get_builtin_type(gdbarch)->builtin_long);
	  write_exp_elt_longcst (number);
	  write_exp_elt_opcode (OP_LONG);

	  p->arg = tmp;

	  if (const_suffix)
	    {
	      if (strncmp (p->arg, const_suffix, const_suffix_len) == 0)
		p->arg += const_suffix_len;
	      else
		error (_("Invalid constant suffix on expression `%s'."),
		       p->saved_arg);
	    }
	}
      else if (reg_ind_prefix
	       && strncmp (tmp, reg_ind_prefix, reg_ind_prefix_len) == 0)
	stap_parse_register_operand (p);
      else
	error (_("Unknown numeric token on expression `%s'."),
	       p->saved_arg);
    }
  else if (const_prefix
	   && strncmp (p->arg, const_prefix, const_prefix_len) == 0)
    {
      /* We are dealing with a numeric constant.  */
      long number;
      char *endp;

      p->arg += const_prefix_len;
      number = strtol (p->arg, &endp, 10);
      p->arg = endp;

      write_exp_elt_opcode (OP_LONG);
      write_exp_elt_type(get_builtin_type(gdbarch)->builtin_long);
      write_exp_elt_longcst (number);
      write_exp_elt_opcode (OP_LONG);

      if (const_suffix)
	{
	  if (strncmp (p->arg, const_suffix, const_suffix_len) == 0)
	    p->arg += const_suffix_len;
	  else
	    error (_("Invalid constant suffix on expression `%s'."),
		   p->saved_arg);
	}
    }
  else if ((reg_prefix
	    && strncmp (p->arg, reg_prefix, reg_prefix_len) == 0)
	   || (reg_ind_prefix
	       && strncmp (p->arg, reg_ind_prefix, reg_ind_prefix_len) == 0))
    stap_parse_register_operand (p);
  else
    error (_("Operator `%c' not recognized on expression `%s'."),
	   *p->arg, p->saved_arg);
}

/* This function parses an argument conditionally, based on single or
   non-single operands.  A non-single operand would be a parenthesized
   expression (e.g., `(2 + 1)'), and a single operand is anything that
   starts with `-', `~', `+' (i.e., unary operators), a digit, or
   something recognized by `gdbarch_stap_is_single_operand'.  */

static void
stap_parse_argument_conditionally (struct stap_parse_info *p)
{
  if (*p->arg == '-' || *p->arg == '~' || *p->arg == '+' /* Unary.  */
      || isdigit (*p->arg)
      || gdbarch_stap_is_single_operand (p->gdbarch, p->arg))
    stap_parse_single_operand (p);
  else if (*p->arg == '(')
    {
      /* We are dealing with a parenthesized operand.  It means we
	 have to parse it as it was a separate expression, without
	 left-side or precedence.  */
      ++p->arg;
      p->arg = skip_spaces_const (p->arg);
      ++p->inside_paren_p;

      stap_parse_argument_1 (p, 0, STAP_OPERAND_PREC_NONE);

      --p->inside_paren_p;
      if (*p->arg != ')')
	error (_("Missign close-paren on expression `%s'."),
	       p->saved_arg);

      ++p->arg;
      if (p->inside_paren_p)
	p->arg = skip_spaces_const (p->arg);
    }
  else
    error (_("Cannot parse expression `%s'."), p->saved_arg);
}

/* Helper function for `stap_parse_argument'.  Please, see its comments to
   better understand what this function does.  */

static void
stap_parse_argument_1 (struct stap_parse_info *p, int has_lhs,
		       enum stap_operand_prec prec)
{
  /* This is an operator-precedence parser.

     We work with left- and right-sides of expressions, and
     parse them depending on the precedence of the operators
     we find.  */

  if (p->inside_paren_p)
    p->arg = skip_spaces_const (p->arg);

  if (!has_lhs)
    {
      /* We were called without a left-side, either because this is the
	 first call, or because we were called to parse a parenthesized
	 expression.  It doesn't really matter; we have to parse the
	 left-side in order to continue the process.  */
      stap_parse_argument_conditionally (p);
    }

  /* Start to parse the right-side, and to "join" left and right sides
     depending on the operation specified.

     This loop shall continue until we run out of characters in the input,
     or until we find a close-parenthesis, which means that we've reached
     the end of a sub-expression.  */
  while (p->arg && *p->arg && *p->arg != ')' && !isspace (*p->arg))
    {
      const char *tmp_exp_buf;
      enum exp_opcode opcode;
      enum stap_operand_prec cur_prec;

      if (!stap_is_operator (p->arg))
	error (_("Invalid operator `%c' on expression `%s'."), *p->arg,
	       p->saved_arg);

      /* We have to save the current value of the expression buffer because
	 the `stap_get_opcode' modifies it in order to get the current
	 operator.  If this operator's precedence is lower than PREC, we
	 should return and not advance the expression buffer pointer.  */
      tmp_exp_buf = p->arg;
      opcode = stap_get_opcode (&tmp_exp_buf);

      cur_prec = stap_get_operator_prec (opcode);
      if (cur_prec < prec)
	{
	  /* If the precedence of the operator that we are seeing now is
	     lower than the precedence of the first operator seen before
	     this parsing process began, it means we should stop parsing
	     and return.  */
	  break;
	}

      p->arg = tmp_exp_buf;
      if (p->inside_paren_p)
	p->arg = skip_spaces_const (p->arg);

      /* Parse the right-side of the expression.  */
      stap_parse_argument_conditionally (p);

      /* While we still have operators, try to parse another
	 right-side, but using the current right-side as a left-side.  */
      while (*p->arg && stap_is_operator (p->arg))
	{
	  enum exp_opcode lookahead_opcode;
	  enum stap_operand_prec lookahead_prec;

	  /* Saving the current expression buffer position.  The explanation
	     is the same as above.  */
	  tmp_exp_buf = p->arg;
	  lookahead_opcode = stap_get_opcode (&tmp_exp_buf);
	  lookahead_prec = stap_get_operator_prec (lookahead_opcode);

	  if (lookahead_prec <= prec)
	    {
	      /* If we are dealing with an operator whose precedence is lower
		 than the first one, just abandon the attempt.  */
	      break;
	    }

	  /* Parse the right-side of the expression, but since we already
	     have a left-side at this point, set `has_lhs' to 1.  */
	  stap_parse_argument_1 (p, 1, lookahead_prec);
	}

      write_exp_elt_opcode (opcode);
    }
}

/* Parse a probe's argument.

   Assuming that:

   LP = literal integer prefix
   LS = literal integer suffix

   RP = register prefix
   RS = register suffix

   RIP = register indirection prefix
   RIS = register indirection suffix

   This routine assumes that arguments' tokens are of the form:

   - [LP] NUMBER [LS]
   - [RP] REGISTER [RS]
   - [RIP] [RP] REGISTER [RS] [RIS]
   - If we find a number without LP, we try to parse it as a literal integer
   constant (if LP == NULL), or as a register displacement.
   - We count parenthesis, and only skip whitespaces if we are inside them.
   - If we find an operator, we skip it.

   This function can also call a special function that will try to match
   unknown tokens.  It will return 1 if the argument has been parsed
   successfully, or zero otherwise.  */

static struct expression *
stap_parse_argument (const char **arg, struct type *atype,
		     struct gdbarch *gdbarch)
{
  struct stap_parse_info p;
  struct cleanup *back_to;

  /* We need to initialize the expression buffer, in order to begin
     our parsing efforts.  The language here does not matter, since we
     are using our own parser.  */
  initialize_expout (10, current_language, gdbarch);
  back_to = make_cleanup (free_current_contents, &expout);

  p.saved_arg = *arg;
  p.arg = *arg;
  p.arg_type = atype;
  p.gdbarch = gdbarch;
  p.inside_paren_p = 0;

  stap_parse_argument_1 (&p, 0, STAP_OPERAND_PREC_NONE);

  discard_cleanups (back_to);

  gdb_assert (p.inside_paren_p == 0);

  /* Casting the final expression to the appropriate type.  */
  write_exp_elt_opcode (UNOP_CAST);
  write_exp_elt_type (atype);
  write_exp_elt_opcode (UNOP_CAST);

  reallocate_expout ();

  p.arg = skip_spaces_const (p.arg);
  *arg = p.arg;

  return expout;
}

/* Function which parses an argument string from PROBE, correctly splitting
   the arguments and storing their information in properly ways.

   Consider the following argument string (x86 syntax):

   `4@%eax 4@$10'

   We have two arguments, `%eax' and `$10', both with 32-bit unsigned bitness.
   This function basically handles them, properly filling some structures with
   this information.  */

static void
stap_parse_probe_arguments (struct stap_probe *probe)
{
  const char *cur;
  struct gdbarch *gdbarch = get_objfile_arch (probe->p.objfile);

  gdb_assert (!probe->args_parsed);
  cur = probe->args_u.text;
  probe->args_parsed = 1;
  probe->args_u.vec = NULL;

  if (!cur || !*cur || *cur == ':')
    return;

  while (*cur)
    {
      struct stap_probe_arg arg;
      enum stap_arg_bitness b;
      int got_minus = 0;
      struct expression *expr;

      memset (&arg, 0, sizeof (arg));

      /* We expect to find something like:

	 N@OP

	 Where `N' can be [+,-][4,8].  This is not mandatory, so
	 we check it here.  If we don't find it, go to the next
	 state.  */
      if ((*cur == '-' && cur[1] && cur[2] != '@')
	  && cur[1] != '@')
	arg.bitness = STAP_ARG_BITNESS_UNDEFINED;
      else
	{
	  if (*cur == '-')
	    {
	      /* Discard the `-'.  */
	      ++cur;
	      got_minus = 1;
	    }

	  if (*cur == '4')
	    b = (got_minus ? STAP_ARG_BITNESS_32BIT_SIGNED
		 : STAP_ARG_BITNESS_32BIT_UNSIGNED);
	  else if (*cur == '8')
	    b = (got_minus ? STAP_ARG_BITNESS_64BIT_SIGNED
		 : STAP_ARG_BITNESS_64BIT_UNSIGNED);
	  else
	    {
	      /* We have an error, because we don't expect anything
		 except 4 and 8.  */
	      complaint (&symfile_complaints,
			 _("unrecognized bitness `%c' for probe `%s'"),
			 *cur, probe->p.name);
	      return;
	    }

	  arg.bitness = b;
	  arg.atype = stap_get_expected_argument_type (gdbarch, b);

	  /* Discard the number and the `@' sign.  */
	  cur += 2;
	}

      expr = stap_parse_argument (&cur, arg.atype, gdbarch);

      if (stap_expression_debug)
	dump_raw_expression (expr, gdb_stdlog,
			     "before conversion to prefix form");

      prefixify_expression (expr);

      if (stap_expression_debug)
	dump_prefix_expression (expr, gdb_stdlog);

      arg.aexpr = expr;

      /* Start it over again.  */
      cur = skip_spaces_const (cur);

      VEC_safe_push (stap_probe_arg_s, probe->args_u.vec, &arg);
    }
}

/* Given PROBE, returns the number of arguments present in that probe's
   argument string.  */

static unsigned
stap_get_probe_argument_count (struct probe *probe_generic)
{
  struct stap_probe *probe = (struct stap_probe *) probe_generic;

  gdb_assert (probe_generic->pops == &stap_probe_ops);

  if (!probe->args_parsed)
    {
      if (probe_generic->pops->can_evaluate_probe_arguments (probe_generic))
	stap_parse_probe_arguments (probe);
      else
	{
	  static int have_warned_stap_incomplete = 0;

	  if (!have_warned_stap_incomplete)
	    {
	      warning (_(
"The SystemTap SDT probe support is not fully implemented on this target;\n"
"you will not be able to inspect the arguments of the probes.\n"
"Please report a bug against GDB requesting a port to this target."));
	      have_warned_stap_incomplete = 1;
	    }

	  /* Marking the arguments as "already parsed".  */
	  probe->args_u.vec = NULL;
	  probe->args_parsed = 1;
	}
    }

  gdb_assert (probe->args_parsed);
  return VEC_length (stap_probe_arg_s, probe->args_u.vec);
}

/* Return 1 if OP is a valid operator inside a probe argument, or zero
   otherwise.  */

static int
stap_is_operator (const char *op)
{
  int ret = 1;

  switch (*op)
    {
    case '*':
    case '/':
    case '%':
    case '^':
    case '!':
    case '+':
    case '-':
    case '<':
    case '>':
    case '|':
    case '&':
      break;

    case '=':
      if (op[1] != '=')
	ret = 0;
      break;

    default:
      /* We didn't find any operator.  */
      ret = 0;
    }

  return ret;
}

static struct stap_probe_arg *
stap_get_arg (struct stap_probe *probe, unsigned n)
{
  if (!probe->args_parsed)
    stap_parse_probe_arguments (probe);

  return VEC_index (stap_probe_arg_s, probe->args_u.vec, n);
}

/* Implement the `can_evaluate_probe_arguments' method of probe_ops.  */

static int
stap_can_evaluate_probe_arguments (struct probe *probe_generic)
{
  struct stap_probe *stap_probe = (struct stap_probe *) probe_generic;
  struct gdbarch *gdbarch = get_objfile_arch (stap_probe->p.objfile);

  /* For SystemTap probes, we have to guarantee that the method
     stap_is_single_operand is defined on gdbarch.  If it is not, then it
     means that argument evaluation is not implemented on this target.  */
  return gdbarch_stap_is_single_operand_p (gdbarch);
}

/* Evaluate the probe's argument N (indexed from 0), returning a value
   corresponding to it.  Assertion is thrown if N does not exist.  */

static struct value *
stap_evaluate_probe_argument (struct probe *probe_generic, unsigned n)
{
  struct stap_probe *stap_probe = (struct stap_probe *) probe_generic;
  struct stap_probe_arg *arg;
  int pos = 0;

  gdb_assert (probe_generic->pops == &stap_probe_ops);

  arg = stap_get_arg (stap_probe, n);
  return evaluate_subexp_standard (arg->atype, arg->aexpr, &pos, EVAL_NORMAL);
}

/* Compile the probe's argument N (indexed from 0) to agent expression.
   Assertion is thrown if N does not exist.  */

static void
stap_compile_to_ax (struct probe *probe_generic, struct agent_expr *expr,
		    struct axs_value *value, unsigned n)
{
  struct stap_probe *stap_probe = (struct stap_probe *) probe_generic;
  struct stap_probe_arg *arg;
  union exp_element *pc;

  gdb_assert (probe_generic->pops == &stap_probe_ops);

  arg = stap_get_arg (stap_probe, n);

  pc = arg->aexpr->elts;
  gen_expr (arg->aexpr, &pc, expr, value);

  require_rvalue (expr, value);
  value->type = arg->atype;
}

/* Destroy (free) the data related to PROBE.  PROBE memory itself is not feed
   as it is allocated from OBJFILE_OBSTACK.  */

static void
stap_probe_destroy (struct probe *probe_generic)
{
  struct stap_probe *probe = (struct stap_probe *) probe_generic;

  gdb_assert (probe_generic->pops == &stap_probe_ops);

  if (probe->args_parsed)
    {
      struct stap_probe_arg *arg;
      int ix;

      for (ix = 0; VEC_iterate (stap_probe_arg_s, probe->args_u.vec, ix, arg);
	   ++ix)
	xfree (arg->aexpr);
      VEC_free (stap_probe_arg_s, probe->args_u.vec);
    }
}



/* This is called to compute the value of one of the $_probe_arg*
   convenience variables.  */

static struct value *
compute_probe_arg (struct gdbarch *arch, struct internalvar *ivar,
		   void *data)
{
  struct frame_info *frame = get_selected_frame (_("No frame selected"));
  CORE_ADDR pc = get_frame_pc (frame);
  int sel = (int) (uintptr_t) data;
  struct probe *pc_probe;
  const struct sym_probe_fns *pc_probe_fns;
  unsigned n_args;

  /* SEL == -1 means "_probe_argc".  */
  gdb_assert (sel >= -1);

  pc_probe = find_probe_by_pc (pc);
  if (pc_probe == NULL)
    error (_("No SystemTap probe at PC %s"), core_addr_to_string (pc));

  gdb_assert (pc_probe->objfile != NULL);
  gdb_assert (pc_probe->objfile->sf != NULL);
  gdb_assert (pc_probe->objfile->sf->sym_probe_fns != NULL);

  pc_probe_fns = pc_probe->objfile->sf->sym_probe_fns;

  n_args = pc_probe_fns->sym_get_probe_argument_count (pc_probe);
  if (sel == -1)
    return value_from_longest(get_builtin_type(arch)->builtin_int, n_args);

  if (sel >= n_args)
    error (_("Invalid probe argument %d -- probe has %u arguments available"),
	   sel, n_args);

  return pc_probe_fns->sym_evaluate_probe_argument (pc_probe, sel);
}

/* This is called to compile one of the $_probe_arg* convenience
   variables into an agent expression.  */

static void
compile_probe_arg (struct internalvar *ivar, struct agent_expr *expr,
		   struct axs_value *value, void *data)
{
  CORE_ADDR pc = expr->scope;
  int sel = (int) (uintptr_t) data;
  struct probe *pc_probe;
  const struct sym_probe_fns *pc_probe_fns;
  int n_args;

  /* SEL == -1 means "_probe_argc".  */
  gdb_assert (sel >= -1);

  pc_probe = find_probe_by_pc (pc);
  if (pc_probe == NULL)
    error (_("No SystemTap probe at PC %s"), core_addr_to_string (pc));

  gdb_assert (pc_probe->objfile != NULL);
  gdb_assert (pc_probe->objfile->sf != NULL);
  gdb_assert (pc_probe->objfile->sf->sym_probe_fns != NULL);

  pc_probe_fns = pc_probe->objfile->sf->sym_probe_fns;

  n_args = pc_probe_fns->sym_get_probe_argument_count (pc_probe);

  if (sel == -1)
    {
      value->kind = axs_rvalue;
      value->type = get_builtin_type(expr->gdbarch)->builtin_int;
      ax_const_l (expr, n_args);
      return;
    }

  gdb_assert (sel >= 0);
  if (sel >= n_args)
    error (_("Invalid probe argument %d -- probe has %d arguments available"),
	   sel, n_args);

  pc_probe_fns->sym_compile_to_ax (pc_probe, expr, value, sel);
}



/* Set or clear a SystemTap semaphore.  ADDRESS is the semaphore's
   address.  SET is zero if the semaphore should be cleared, or one
   if it should be set.  This is a helper function for `stap_semaphore_down'
   and `stap_semaphore_up'.  */

static void
stap_modify_semaphore (CORE_ADDR address, int set, struct gdbarch *gdbarch)
{
  gdb_byte bytes[sizeof (LONGEST)];
  /* The ABI specifies "unsigned short".  */
  struct type *type = get_builtin_type(gdbarch)->builtin_unsigned_short;
  ULONGEST value;

  if (address == 0)
    return;

  /* Swallow errors.  */
  if (target_read_memory (address, bytes, TYPE_LENGTH (type)) != 0)
    {
      warning (_("Could not read the value of a SystemTap semaphore."));
      return;
    }

  value = extract_unsigned_integer (bytes, TYPE_LENGTH (type),
				    gdbarch_byte_order (gdbarch));
  /* Note that we explicitly don't worry about overflow or
     underflow.  */
  if (set)
    ++value;
  else
    --value;

  store_unsigned_integer (bytes, TYPE_LENGTH (type),
			  gdbarch_byte_order (gdbarch), value);

  if (target_write_memory (address, bytes, TYPE_LENGTH (type)) != 0)
    warning (_("Could not write the value of a SystemTap semaphore."));
}

/* Set a SystemTap semaphore.  SEM is the semaphore's address.  Semaphores
   act as reference counters, so calls to this function must be paired with
   calls to `stap_semaphore_down'.

   This function and `stap_semaphore_down' race with another tool changing
   the probes, but that is too rare to care.  */

static void
stap_set_semaphore (struct probe *probe_generic, struct gdbarch *gdbarch)
{
  struct stap_probe *probe = (struct stap_probe *) probe_generic;

  gdb_assert (probe_generic->pops == &stap_probe_ops);

  stap_modify_semaphore (probe->sem_addr, 1, gdbarch);
}

/* Clear a SystemTap semaphore.  SEM is the semaphore's address.  */

static void
stap_clear_semaphore (struct probe *probe_generic, struct gdbarch *gdbarch)
{
  struct stap_probe *probe = (struct stap_probe *) probe_generic;

  gdb_assert (probe_generic->pops == &stap_probe_ops);

  stap_modify_semaphore (probe->sem_addr, 0, gdbarch);
}

/* Implementation of `$_probe_arg*' set of variables.  */

static const struct internalvar_funcs probe_funcs =
{
  compute_probe_arg,
  compile_probe_arg,
  NULL
};

/* Helper function that parses the information contained in a
   SystemTap's probe.  Basically, the information consists in:

   - Probe's PC address;
   - Link-time section address of `.stapsdt.base' section;
   - Link-time address of the semaphore variable, or ZERO if the
     probe doesn't have an associated semaphore;
   - Probe's provider name;
   - Probe's name;
   - Probe's argument format
   
   This function returns 1 if the handling was successful, and zero
   otherwise.  */

static void
handle_stap_probe (struct objfile *objfile, struct sdt_note *el,
		   VEC (probe_p) **probesp, CORE_ADDR base)
{
  bfd *abfd = objfile->obfd;
  int size = bfd_get_arch_size (abfd) / 8;
  struct gdbarch *gdbarch = get_objfile_arch (objfile);
  struct type *ptr_type = get_builtin_type(gdbarch)->builtin_data_ptr;
  CORE_ADDR base_ref;
  const char *probe_args = NULL;
  struct stap_probe *ret;

  ret = obstack_alloc (&objfile->objfile_obstack, sizeof (*ret));
  ret->p.pops = &stap_probe_ops;
  ret->p.objfile = objfile;

  /* Provider and the name of the probe.  */
  ret->p.provider = (char *) &el->data[3 * size];
  ret->p.name = memchr (ret->p.provider, '\0',
			(char *) el->data + el->size - ret->p.provider);
  /* Making sure there is a name.  */
  if (!ret->p.name)
    {
      complaint (&symfile_complaints, _("corrupt probe name when "
					"reading `%s'"),
		 objfile_name (objfile));

      /* There is no way to use a probe without a name or a provider, so
	 returning zero here makes sense.  */
      return;
    }
  else
    ++ret->p.name;

  /* Retrieving the probe's address.  */
  ret->p.address = extract_typed_address (&el->data[0], ptr_type);

  /* Link-time sh_addr of `.stapsdt.base' section.  */
  base_ref = extract_typed_address (&el->data[size], ptr_type);

  /* Semaphore address.  */
  ret->sem_addr = extract_typed_address (&el->data[2 * size], ptr_type);

  ret->p.address += (ANOFFSET (objfile->section_offsets,
			       SECT_OFF_TEXT (objfile))
		     + base - base_ref);
  if (ret->sem_addr)
    ret->sem_addr += (ANOFFSET (objfile->section_offsets,
				SECT_OFF_DATA (objfile))
		      + base - base_ref);

  /* Arguments.  We can only extract the argument format if there is a valid
     name for this probe.  */
  probe_args = memchr (ret->p.name, '\0',
		       (char *) el->data + el->size - ret->p.name);

  if (probe_args != NULL)
    ++probe_args;

  if (probe_args == NULL || (memchr (probe_args, '\0',
				     (char *) el->data + el->size - ret->p.name)
			     != el->data + el->size - 1))
    {
      complaint (&symfile_complaints, _("corrupt probe argument when "
					"reading `%s'"),
		 objfile_name (objfile));
      /* If the argument string is NULL, it means some problem happened with
	 it.  So we return 0.  */
      return;
    }

  ret->args_parsed = 0;
  ret->args_u.text = (void *) probe_args;

  /* Successfully created probe.  */
  VEC_safe_push (probe_p, *probesp, (struct probe *) ret);
}

/* Helper function which tries to find the base address of the SystemTap
   base section named STAP_BASE_SECTION_NAME.  */

static void
get_stap_base_address_1 (bfd *abfd, asection *sect, void *obj)
{
  asection **ret = obj;

  if ((sect->flags & (SEC_DATA | SEC_ALLOC | SEC_HAS_CONTENTS))
      && sect->name && !strcmp (sect->name, STAP_BASE_SECTION_NAME))
    *ret = sect;
}

/* Helper function which iterates over every section in the BFD file,
   trying to find the base address of the SystemTap base section.
   Returns 1 if found (setting BASE to the proper value), zero otherwise.  */

static int
get_stap_base_address (bfd *obfd, bfd_vma *base)
{
  asection *ret = NULL;

  bfd_map_over_sections (obfd, get_stap_base_address_1, (void *) &ret);

  if (!ret)
    {
      complaint (&symfile_complaints, _("could not obtain base address for "
					"SystemTap section on objfile `%s'."),
		 obfd->filename);
      return 0;
    }

  if (base)
    *base = ret->vma;

  return 1;
}

/* Helper function for `elf_get_probes', which gathers information about all
   SystemTap probes from OBJFILE.  */

static void
stap_get_probes (VEC (probe_p) **probesp, struct objfile *objfile)
{
  /* If we are here, then this is the first time we are parsing the
     SystemTap probe's information.  We basically have to count how many
     probes the objfile has, and then fill in the necessary information
     for each one.  */
  bfd *obfd = objfile->obfd;
  bfd_vma base;
  struct sdt_note *iter;
  unsigned save_probesp_len = VEC_length (probe_p, *probesp);

  if (objfile->separate_debug_objfile_backlink != NULL)
    {
      /* This is a .debug file, not the objfile itself.  */
      return;
    }

  if (!elf_tdata (obfd)->sdt_note_head)
    {
      /* There isn't any probe here.  */
      return;
    }

  if (!get_stap_base_address (obfd, &base))
    {
      /* There was an error finding the base address for the section.
	 Just return NULL.  */
      return;
    }

  /* Parsing each probe's information.  */
  for (iter = elf_tdata (obfd)->sdt_note_head; iter; iter = iter->next)
    {
      /* We first have to handle all the information about the
	 probe which is present in the section.  */
      handle_stap_probe (objfile, iter, probesp, base);
    }

  if (save_probesp_len == VEC_length (probe_p, *probesp))
    {
      /* If we are here, it means we have failed to parse every known
	 probe.  */
      complaint (&symfile_complaints, _("could not parse SystemTap probe(s) "
					"from inferior"));
      return;
    }
}

static void
stap_relocate (struct probe *probe_generic, CORE_ADDR delta)
{
  struct stap_probe *probe = (struct stap_probe *) probe_generic;

  gdb_assert (probe_generic->pops == &stap_probe_ops);

  probe->p.address += delta;
  if (probe->sem_addr)
    probe->sem_addr += delta;
}

static int
stap_probe_is_linespec (const char **linespecp)
{
  static const char *const keywords[] = { "-pstap", "-probe-stap", NULL };

  return probe_is_linespec_by_keyword (linespecp, keywords);
}

static void
stap_gen_info_probes_table_header (VEC (info_probe_column_s) **heads)
{
  info_probe_column_s stap_probe_column;

  stap_probe_column.field_name = "semaphore";
  stap_probe_column.print_name = _("Semaphore");

  VEC_safe_push (info_probe_column_s, *heads, &stap_probe_column);
}

static void
stap_gen_info_probes_table_values (struct probe *probe_generic,
				   VEC (const_char_ptr) **ret)
{
  struct stap_probe *probe = (struct stap_probe *) probe_generic;
  struct gdbarch *gdbarch;
  const char *val = NULL;

  gdb_assert (probe_generic->pops == &stap_probe_ops);

  gdbarch = get_objfile_arch (probe->p.objfile);

  if (probe->sem_addr)
    val = print_core_address (gdbarch, probe->sem_addr);

  VEC_safe_push (const_char_ptr, *ret, val);
}

/* SystemTap probe_ops.  */

static const struct probe_ops stap_probe_ops =
{
  stap_probe_is_linespec,
  stap_get_probes,
  stap_relocate,
  stap_get_probe_argument_count,
  stap_can_evaluate_probe_arguments,
  stap_evaluate_probe_argument,
  stap_compile_to_ax,
  stap_set_semaphore,
  stap_clear_semaphore,
  stap_probe_destroy,
  stap_gen_info_probes_table_header,
  stap_gen_info_probes_table_values,
};

/* Implementation of the `info probes stap' command.  */

static void
info_probes_stap_command (char *arg, int from_tty)
{
  info_probes_for_ops (arg, from_tty, &stap_probe_ops);
}

void _initialize_stap_probe (void);

void
_initialize_stap_probe (void)
{
  VEC_safe_push (probe_ops_cp, all_probe_ops, &stap_probe_ops);

  add_setshow_zuinteger_cmd ("stap-expression", class_maintenance,
			     &stap_expression_debug,
			     _("Set SystemTap expression debugging."),
			     _("Show SystemTap expression debugging."),
			     _("When non-zero, the internal representation "
			       "of SystemTap expressions will be printed."),
			     NULL,
			     show_stapexpressiondebug,
			     &setdebuglist, &showdebuglist);

  create_internalvar_type_lazy ("_probe_argc", &probe_funcs,
				(void *) (uintptr_t) -1);
  create_internalvar_type_lazy ("_probe_arg0", &probe_funcs,
				(void *) (uintptr_t) 0);
  create_internalvar_type_lazy ("_probe_arg1", &probe_funcs,
				(void *) (uintptr_t) 1);
  create_internalvar_type_lazy ("_probe_arg2", &probe_funcs,
				(void *) (uintptr_t) 2);
  create_internalvar_type_lazy ("_probe_arg3", &probe_funcs,
				(void *) (uintptr_t) 3);
  create_internalvar_type_lazy ("_probe_arg4", &probe_funcs,
				(void *) (uintptr_t) 4);
  create_internalvar_type_lazy ("_probe_arg5", &probe_funcs,
				(void *) (uintptr_t) 5);
  create_internalvar_type_lazy ("_probe_arg6", &probe_funcs,
				(void *) (uintptr_t) 6);
  create_internalvar_type_lazy ("_probe_arg7", &probe_funcs,
				(void *) (uintptr_t) 7);
  create_internalvar_type_lazy ("_probe_arg8", &probe_funcs,
				(void *) (uintptr_t) 8);
  create_internalvar_type_lazy ("_probe_arg9", &probe_funcs,
				(void *) (uintptr_t) 9);
  create_internalvar_type_lazy ("_probe_arg10", &probe_funcs,
				(void *) (uintptr_t) 10);
  create_internalvar_type_lazy ("_probe_arg11", &probe_funcs,
				(void *) (uintptr_t) 11);

  add_cmd ("stap", class_info, info_probes_stap_command,
	   _("\
Show information about SystemTap static probes.\n\
Usage: info probes stap [PROVIDER [NAME [OBJECT]]]\n\
Each argument is a regular expression, used to select probes.\n\
PROVIDER matches probe provider names.\n\
NAME matches the probe names.\n\
OBJECT matches the executable or shared library name."),
	   info_probes_cmdlist_get ());

}
