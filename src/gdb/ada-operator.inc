/* Ada language operator definitions for GDB, the GNU debugger. -*- C -*-

   Copyright (C) 1992, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005,
   2007, 2008, 2009 Free Software Foundation, Inc.

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

    /* X IN A'RANGE(N).  N is an immediate operand, surrounded by 
       BINOP_IN_BOUNDS before and after.  A is an array, X an index 
       value.  Evaluates to true iff X is within range of the Nth
       dimension (1-based) of A.  (A multi-dimensional array
       type is represented as array of array of ...) */
    BINOP_IN_BOUNDS = OP_EXTENDED0,

    /* X IN L .. U.  True iff L <= X <= U.  */
    TERNOP_IN_RANGE,

    /* Ada attributes ('Foo). */
    OP_ATR_FIRST,
    OP_ATR_LAST,
    OP_ATR_LENGTH,
    OP_ATR_IMAGE,
    OP_ATR_MAX,
    OP_ATR_MIN,
    OP_ATR_MODULUS,
    OP_ATR_POS,
    OP_ATR_SIZE,
    OP_ATR_TAG,
    OP_ATR_VAL,

    /* Ada type qualification.  It is encoded as for UNOP_CAST, above, 
       and denotes the TYPE'(EXPR) construct. */
    UNOP_QUAL,

    /* X IN TYPE.  The `TYPE' argument is immediate, with 
       UNOP_IN_RANGE before and after it. True iff X is a member of 
       type TYPE (typically a subrange). */
    UNOP_IN_RANGE,

    /* An aggregate.   A single immediate operand, N>0, gives
       the number of component specifications that follow.  The
       immediate operand is followed by a second OP_AGGREGATE.  
       Next come N component specifications.  A component
       specification is either an OP_OTHERS (others=>...), an
       OP_CHOICES (for named associations), or other expression (for
       positional aggregates only).  Aggregates currently
       occur only as the right sides of assignments. */
    OP_AGGREGATE,

    /* An others clause.  Followed by a single expression. */
    OP_OTHERS,

    /* An aggregate component association.  A single immediate operand, N, 
       gives the number of choices that follow.  This is followed by a second
       OP_CHOICES operator.  Next come N operands, each of which is an
       expression, an OP_DISCRETE_RANGE, or an OP_NAME---the latter 
       for a simple name that must be a record component name and does 
       not correspond to a single existing symbol.  After the N choice 
       indicators comes an expression giving the value.

       In an aggregate such as (X => E1, ...), where X is a simple
       name, X could syntactically be either a component_selector_name 
       or an expression used as a discrete_choice, depending on the
       aggregate's type context.  Since this is not known at parsing
       time, we don't attempt to disambiguate X if it has multiple
       definitions, but instead supply an OP_NAME.  If X has a single
       definition, we represent it with an OP_VAR_VALUE, even though
       it may turn out to be within a record aggregate.  Aggregate 
       evaluation can use either OP_NAMEs or OP_VAR_VALUEs to get a
       record field name, and can evaluate OP_VAR_VALUE normally to
       get its value as an expression.  Unfortunately, we lose out in
       cases where X has multiple meanings and is part of an array
       aggregate.  I hope these are not common enough to annoy users,
       who can work around the problem in any case by putting
       parentheses around X. */
    OP_CHOICES,

    /* A positional aggregate component association.  The operator is 
       followed by a single integer indicating the position in the 
       aggregate (0-based), followed by a second OP_POSITIONAL.  Next 
       follows a single expression giving the component value.  */
    OP_POSITIONAL,

    /* A range of values.  Followed by two expressions giving the
       upper and lower bounds of the range. */
    OP_DISCRETE_RANGE,

    /* End marker */
    OP_ADA_LAST,
