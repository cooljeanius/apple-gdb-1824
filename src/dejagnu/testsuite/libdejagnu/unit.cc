// unit.cc -- This is a test case for the Dejagnu.h classes.
// Copyright 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009,
// 2010 Free Software Foundation, Inc.

// This file is part of DejaGnu.

// DejaGnu is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.

// DejaGnu is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with DejaGnu; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning not including "config.h".
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif // HAVE_CONFIG_H
#include <sys/types.h>
#include <iostream>
#include <regex.h>
#include <string>
#include <string.h>
#include <fstream>
#include <set>
#include <dejagnu.h>
#ifndef __DEJAGNU_H__
# include "../../dejagnu.h"
#endif /* !__DEJAGNU_H__ */

using namespace std;

TestState runtest;
TestState test;

class TestClass1 {
public:
	string tname;
	unsigned int tnum;
};

TestClass1 testClass1, testClass2, testClass3;
TestClass1 *testClassPtr;

// We have to so this silly crap with renaming the output string, so the generic
// Tcl code that looks for the output state gets confused, and records random
// duplicate messages.
const char *os1[] = {
    "FAI: ",
    "PAS: ",
    "UNT: ",
    "UNR: "
};

const char *os2[] = {
    "FAILED: ",
    "PASSED: ",
    "UNTESTED: ",
    "UNRESOLVED: "
};

int
main(int argc, char *argv[]) {
#if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
# pragma unused (argc, argv)
#endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
    regex_t regex_pat;

#ifdef ALLOW_UNUSED_VARIABLES
    char **os2 = outstate;
#endif /* ALLOW_UNUSED_VARIABLES */
    outstate = os1;
    streambuf *pbuf;

    // Replace the output buffer for cout, so we can examine it to
    // see what was displayed. Otherwise, there is no way we can test
    // the logging functions completely.
    char bbuuff[5120];
#ifdef HAVE_STL3
    cout.rdbuf()->pubsetbuf(bbuuff, 5120);
#else
    cout.rdbuf()->setbuf(bbuuff, 5120);
#endif /* HAVE_STL3 */
    pbuf = cout.rdbuf();

    testClass1.tname = "testType1";
    testClass1.tnum = 1;
    testClass2.tname = "testType2";
    testClass2.tnum = 2;
    testClass3.tname = "testType3";
    testClass3.tnum = 3;

    // Test the pass message:
    test.pass("bogus pass message for testing");
    outstate = os2;
    cout.rdbuf(pbuf);
    if (strncmp(bbuuff, "\tPAS: bogus pass message", 22) == 0) {
	runtest.pass("Pass message");
    } else {
	runtest.fail("Pass message");
    }

    // Test the fail message:
    outstate = os1;
    test.fail("bogus fail message for testing");
    cout.flush();
    cout.rdbuf(pbuf);
    outstate = os2;
    if (strncmp(bbuuff, "\tFAI: bogus fail message", 22) == 0) {
	runtest.pass("Fail message");
    } else {
	runtest.fail("Fail message");
    }

    // Test the untested message:
    outstate = os1;
    test.untested("bogus untested message for testing");
    cout.flush();
    cout.rdbuf(pbuf);
    outstate = os2;
    if (strncmp(bbuuff, "\tUNT: bogus untested message", 21) == 0) {
	runtest.pass("Untested message");
    } else {
	runtest.fail("Untested message");
    }

    // Test the unresolved message:
    outstate = os1;
    test.unresolved("bogus unresolved message for testing");
    cout.flush();
    cout.rdbuf(pbuf);
    outstate = os2;
    if (strncmp(bbuuff, "\tUNR: bogus unresolved message", 21) == 0) {
	runtest.pass("Unresolved message");
    } else {
	runtest.fail("Unresolved message");
    }

    // Make sure we got everything in the totals:
    regcomp(&regex_pat,
            "\r\n\t#passed.*#real failed.*#untested.*#unresolved",
            (REG_NOSUB | REG_NEWLINE));

    test.totals();
    cout.rdbuf(pbuf);
    if (regexec(&regex_pat, bbuuff, 0, (regmatch_t *)NULL, 0)) {
	runtest.pass("Totals message");
    } else {
	runtest.fail("Totals message");
    }

    return 0;
}

/* EOF */
