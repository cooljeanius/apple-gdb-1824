%%comments:
Copyright (c) 2001-2012 by Bruce Korb - all rights reserved

Permission is granted to copy, distribute and/or modify this document
under the terms of the GNU Free Documentation License, Version 1.1 or
any later version published by the Free Software Foundation; with no
Invariant Sections, with no Front-Cover Texts, and with no Back-Cover
Texts. A copy of the license is included in the file COPYING.

%%name: Autogen

%%short-description: Automated program and text generation

%%full-description: Autogen is a tool designed for generating program
files that contain repetitive text with varied substitutions. its
goal is to simplify the maintenance of programs that have large
amounts of repetitive text. This is particularly valuable
if there are several blocks of such text that must be kept
synchronized. The follwing programs and libraries are also included:

<P>AutoOpts- This is a link library that can be redistributed with any
project.  It completely automates the process of parsing command line
options, configuration files, environment variables, usage text, man
pages, and the invoking section of an info doc. It currently produces
option processing code for C, C++, Guile inner-main programs and shell
scripts.

<P>AutoFSM- Where it is possible to determine a state transition type
(token code) largely without reference to the current state, this
template will produce a transition table and prototype finite state
machine. If you must know current state to determine the transition
type, this is not useful.

<P>getdefs- Extracts AutoGen definitions from stylized commentes
embedded in source code.

<P>columns- tabularizes lists for improved output appearance.
See also ls(1).

<P>AutoXDR- is an affiliated download.
NFSv4 specifies that its remote procedure calls be batched. Using
an enhanced XDR specification and the AutoXDR templates, AutoGen
generates all the grunge code for marshalling and unmarshalling
the arguments on both sides of the RPC request.

%%category: software development, program build automation

%%license: GPL, LGPL, BSD, public domain

%%license verified by: Janet Casey <jcasey@gnu.org>

%%license verified on: 2001-01-31

%%maintainer: Bruce Korb <bkorb@gnu.org>

%%updated: 2005-02-01

%%keywords: RPC, rpcgen, NFSv4, finite state machine, FSM, Guile, M4

%%interface: Command line

%%programs: autogen, getdefs, columns

%%libraries: AutoOpts, AutoXDR, AutoFSM

%%GNU: yes

%%web-page: http://www.gnu.org/software/autogen

%%support:

%%doc: Developer's reference manual available from
http://www.gnu.org/manual/autogen/autogen.html

%%developers: Bruce Korb <bkorb@gnu.org>, Gary V. Vaughan <gvv@technie.com>

%%contributors:

%%sponsors:

%%source-tarball: ftp://ftp.gnu.org/pub/gnu/autogen/autogen-5.3.2.tar.gz

%%source-info: http://prdownloads.sourceforge.net/autogen/

%%source-template:

%%debian: http://packages.debian.org/unstable/devel/autogen.html

%%rpm:

%%repository: cvs.AutoGen.sourceforge.net:/cvsroot/AutoGen

%%related: Guile

%%source-language: C, Guile, Bourne shell

%%supported-languages:

%%use-requirements: Guile, Bourne-like shell, POSIX environment

%%build-prerequisites: Guile, ANSI C, Bourne-like shell, POSIX environment

%%weak-prerequisites:

%%source-prerequisites:

%%version: 5.3.2 stable released 2002-02-25

%%announce-list: <autogen-announce@lists.sourceforge.net>

%%announce-news:

%%help-list: <autogen-users@lists.sourceforge.net>

%%help-news:

%%irc-help-channel:

%%dev-list: <autogen-devel@lists.sourceforge.net>

%%dev-news:

%%irc-dev-channel:

%%bug-list: <autogen-users@lists.sourceforge.net>

%%bug-database:

%%Entry Written By: Janet Casey <jcasey@gnu.org>













