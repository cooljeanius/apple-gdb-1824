/* eval-plural.h
   Plural expression evaluation.
   Copyright (C) 2000-2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - 5th Floor, Boston, MA 02110-1301,
   USA.  */

#ifndef STATIC
# define STATIC static
#endif /* !STATIC */

#ifndef PARAMS
# if __STDC__ || defined __GNUC__ || defined __SUNPRO_C || defined __cplusplus || __PROTOTYPES
#  define PARAMS(args) args
# else
#  define PARAMS(args) ()
# endif /* __STDC__ */
#endif /* !PARAMS */

#ifndef GCC_VERSION
# if defined(__GNUC__) && defined(__GNUC_MINOR__)
#  define GCC_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
# else
#  define GCC_VERSION 0
# endif /* __GNUC__ && __GNUC_MINOR__ */
#endif /* GCC_VERSION */

#if !defined __has_builtin
# define __has_builtin(x) 0
#endif

/* Use intl_unreachable() to mark unreachable locations, like an
 * unreachable default case of a switch. */
#if (GCC_VERSION >= 4005) || __has_builtin(__builtin_unreachable)
# define intl_unreachable() __builtin_unreachable()
#else
# define intl_unreachable()
#endif

/* Evaluate the plural expression and return an index value.  */
STATIC unsigned long int plural_eval PARAMS((struct expression *pexp,
                                             unsigned long int n))
     internal_function;

STATIC
unsigned long int
internal_function
plural_eval(struct expression *pexp, unsigned long int n)
{
  switch (pexp->nargs)
    {
    case 0:
      switch (pexp->operation) {
	case var:
	  return n;
	case num:
	  return pexp->val.num;
	default:
	  break;
      }
      intl_unreachable();
      /* NOTREACHED */
      break;
    case 1:
      {
	/* pexp->operation must be lnot: */
	unsigned long int arg = plural_eval(pexp->val.args[0], n);
	return ! arg;
      }
    case 2:
      {
	unsigned long int leftarg = plural_eval(pexp->val.args[0], n);
	if (pexp->operation == lor) {
	  return (leftarg || plural_eval(pexp->val.args[1], n));
	} else if (pexp->operation == land) {
	  return (leftarg && plural_eval(pexp->val.args[1], n));
	} else {
	    unsigned long int rightarg = plural_eval(pexp->val.args[1], n);

	    switch (pexp->operation) {
	      case mult:
		return (leftarg * rightarg);
	      case divide:
#if (!defined(INTDIV0_RAISES_SIGFPE) || !INTDIV0_RAISES_SIGFPE)
                if (rightarg == 0) {
		  raise(SIGFPE);
                }
#endif /* !INTDIV0_RAISES_SIGFPE */
		return (leftarg / rightarg);
	      case module:
#if (!defined(INTDIV0_RAISES_SIGFPE) || !INTDIV0_RAISES_SIGFPE)
                if (rightarg == 0) {
		  raise(SIGFPE);
                }
#endif /* !INTDIV0_RAISES_SIGFPE */
		return (leftarg % rightarg);
	      case plus:
		return (leftarg + rightarg);
	      case minus:
		return (leftarg - rightarg);
	      case less_than:
		return (leftarg < rightarg);
	      case greater_than:
		return (leftarg > rightarg);
	      case less_or_equal:
		return (leftarg <= rightarg);
	      case greater_or_equal:
		return (leftarg >= rightarg);
	      case equal:
		return (leftarg == rightarg);
	      case not_equal:
		return (leftarg != rightarg);
	      default:
		break;
            }
        }
        intl_unreachable();
	/* NOTREACHED */
	break;
      }
    case 3:
      {
	/* pexp->operation must be qmop: */
	unsigned long int boolarg = plural_eval(pexp->val.args[0], n);
	return plural_eval(pexp->val.args[boolarg ? 1 : 2], n);
      }
    default:
      intl_unreachable();
      /* NOTREACHED */
      break;
    }
  intl_unreachable();
  /* NOTREACHED */
  return 0;
}

/* EOF */
