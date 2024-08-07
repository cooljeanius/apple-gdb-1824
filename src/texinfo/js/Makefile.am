## Process this file with automake to produce Makefile.in
# Copyright (C) 2017-2022  Free Software Foundation, Inc.
#
# This file is part of GNU Texinfo.
#
# GNU Texinfo is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# GNU Texinfo is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Texinfo.  If not, see <http://www.gnu.org/licenses/>.


phony =
checktargs =

CLEANFILES =

if HAVE_ESLINT
phony += lint
checktargs += lint
lint: build-aux/eslint.json info.js
	$(ESLINT) -c build-aux/eslint.json info.js -f unix
endif

if HAVE_TYPESCRIPT
phony += check-types
checktargs += @MAINT@ check-types
check-types: info.js
	$(TSC) --allowJs --checkJS --noEmit --lib dom,es2017 info.js || \
	  (stat info.js && sync && wc -l info.js && sync && du info.js)
endif

if HAVE_UGLIFYJS
phony += minify
checktargs += minify
CLEANFILES += info.min.js
minify: info.js
	$(UGLIFYJS) info.js -o info.min.js --mangle --compress
endif

if HAVE_MODERNIZR
phony += modernizr
checktargs += modernizr
modernizr: build-aux/modernizr.json
	$(MODERNIZR) -c build-aux/modernizr.json
endif

.PHONY: $(phony)
check-local: $(checktargs)

EXTRA_DIST =

jsdir=$(pkgdatadir)/js

# Javascript files that will be included in the html pages.  The order
# of those files is the order in which they will be included
js_scripts = \
  modernizr.js \
  info.js

dist_js_DATA=$(js_scripts) info.css

EXTRA_DIST += HACKING README TODO \
              build-aux/eslint.json \
              build-aux/modernizr.json \
              modernizr.js \
              package.json \
              tsconfig.json

DISTCLEANFILES = autoscan.log
