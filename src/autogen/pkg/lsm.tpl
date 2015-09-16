[= AutoGen5 template

lsm=autogen.lsm

# Time-stamp:        "2010-02-24 08:37:18 bkorb"

##
##  This file is part of AutoGen.
##  AutoGen Copyright (c) 1992-2012 by Bruce Korb - all rights reserved
##
##  AutoGen is free software: you can redistribute it and/or modify it
##  under the terms of the GNU General Public License as published by the
##  Free Software Foundation, either version 3 of the License, or
##  (at your option) any later version.
##
##  AutoGen is distributed in the hope that it will be useful, but
##  WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
##  See the GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License along
##  with this program.  If not, see <http://www.gnu.org/licenses/>.
=]
Begin4
Title:          AutoGen

Version:        [=version=]

Entered-date:   [=`date +%Y-%m-%d`=]

Description:    AutoGen - [=prog_title=]

                AutoGen is a tool for automatically generating
                arbitrary text files that contain repetitive text
                with varying substitutions.  This is particularly
                useful if you have several types of repetitive
                text that all need to be kept in sync with each
                other.  The goal is to try to simplify the process
                of maintaining repetitive program text.

Keywords:       macro, m4, preprocessor, source generator,
                command line option parser

Author:         bkorb@gnu.org (Bruce Korb)

Maintained-by:  bkorb@gnu.org (Bruce Korb)

Primary-site:   sunsite.unc.edu /pub/Linux/devel
                [= `
cd $top_builddir
set -- autogen*.gz
if [ $# -gt 1 ]
then shift \`expr $# - 1\` ; fi
if [ ! -f $1 ]
then ct=1151
else ct="\`expr \\\\( \\\`wc -c < $1\\\` + 1023 \\\\) / 1024\`"
fi
echo ${ct}K $1
` =]
                1K autogen.lsm

Alternate-site: ftp://ftp.gnu.org/pub/autogen

Original-site:  http://autogen.sourceforge.net

Platforms:      gunzip, Guile and ANSI-C

Copying-policy: GPL and LGPL

End
