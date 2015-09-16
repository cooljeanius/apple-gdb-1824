[= Autogen5 Template -*- Mode: scheme -*-

h
c

# Time-stamp:      "2011-01-28 10:30:50 bkorb"

# This file contains the templates used to generate the
# option descriptions for client programs, and it declares
# the macros used in the templates.

##  This file is part of AutoOpts, a companion to AutoGen.
##  AutoOpts is free software.
##  AutoOpts is Copyright (c) 1992-2012 by Bruce Korb - all rights reserved
##
##  AutoOpts is available under any one of two licenses.  The license
##  in use must be one of these two and the choice is under the control
##  of the user of the license.
##
##   The GNU Lesser General Public License, version 3 or later
##      See the files "COPYING.lgplv3" and "COPYING.gplv3"
##
##   The Modified Berkeley Software Distribution License
##      See the file "COPYING.mbsd"
##
##  These files have the following md5sums:
##
##  43b91e8ca915626ed3818ffb1b71248b COPYING.gplv3
##  06a1a2e4760c90ea5e1dad8dfaac4d39 COPYING.lgplv3
##  66a5cedaf62c4b2637025f049f9b826f COPYING.mbsd

=][= (dne " *  " "/*  ")    =][=

CASE    (suffix)            =][=

== h                        =][=

  INCLUDE "optlib.tlib"     =][=
  INVOKE  init-and-validate =][=
  INVOKE  option-copyright  =][=
  INCLUDE "opthead.tlib"    =][=

== c                        =][=

  (if (exist? "library")
      (out-delete))         =][=

  INVOKE  option-copyright  =][=
  INCLUDE "optcode.tlib"    =][=

  (if (exist? "flag.extract-code")
      (shellf "test -f %1$s.c && rm -f %1$s.c.save" (base-name)))  =][=

ESAC =]
/* [= (out-name) =] ends here */[=

# options.tpl ends here     =]
