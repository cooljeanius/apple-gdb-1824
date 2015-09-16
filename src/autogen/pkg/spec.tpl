[= AutoGen5 Template spec =]
[= #
   Time-stamp:        "2010-02-24 08:36:09 bkorb"

 *  This file is part of AutoGen.
 *  AutoGen Copyright (c) 1992-2012 by Bruce Korb - all rights reserved
 *
 *  AutoGen is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  AutoGen is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program.  If not, see <http://www.gnu.org/licenses/>.
\=]
Summary:    AutoGen - [=prog-title=]
Name:       [= prog-name =]
Version:    [= version =]
Vendor:     [= copyright.owner =] http://www.gnu.org/software/autogen
Release:    [=`echo $AG_MAJOR_VERSION`=]
License:    GPL
Group:      Development/Tools
Source:     ftp://ftp.gnu.org/gnu/autogen/rel[= version =]/autogen-[= version
                =].tar.gz
BuildRoot:  %{_tmppath}/%{name}-root

%description
AutoGen is a tool designed for generating program files that contain
repetitive text with varied substitutions.  Its goal is to simplify the
maintenance of programs that contain large amounts of repetitious text.
This is especially valuable if there are several blocks of such text
that must be kept synchronized in parallel tables.

Some parts are released under different licensing:

libopts LGPL  This is a tear-off, redistributable option processing library
autofsm BSD   This is a template for producing finite state machine programs

The Copyright itself is privately held by Bruce Korb.
Copyright (c) [= copyright.date =] by [= copyright.owner
                =].  All rights reserved.  Licensed under GPL, [=#
                =]version 2 or later.
%prep
%setup -q
chmod -R +rw *

%build
%configure
make CFLAGS="$RPM_OPT_FLAGS"

if [ `id -u` -eq 0 ] && egrep -q ^nobody /etc/passwd
then
    echo "switching to user nobody to run 'make check'"
    chown -R nobody . ; su -c "umask 002; make check || touch FAIL" nobody
else
    make check
fi
[ -f FAIL ] && exit 1

%install
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf ${RPM_BUILD_ROOT}
mkdir -p ${RPM_BUILD_ROOT}
make install DESTDIR=${RPM_BUILD_ROOT}

# IF we have a valid file list OR the build root is _the_ root,
# THEN skip the file list generation.
#
if test \( -f autogen-filelist \
        -a -s autogen-filelist \) \
        -o ${#RPM_BUILD_ROOT} -le 1
then : ; else
  ( cd ${RPM_BUILD_ROOT}
    rm -f usr/share/info/dir
    find . -type f -o -type l | grep -v 'usr/share/doc'
  ) | sed -e 's@^\./@/@' \
          -e'/usr\/share\/info/s,$,.gz,' \
          -e'/usr\/share\/man/s,$,.gz,' \
    | sort \
    > autogen-filelist
fi

%post
/sbin/ldconfig
/sbin/install-info --info-dir=%{_infodir} %{_infodir}/autogen.info*

%preun
/sbin/install-info --delete --info-dir=%{_infodir} %{_infodir}/autogen.info*

%postun -p /sbin/ldconfig

%clean
rm -rf ${RPM_BUILD_ROOT}

%files -f autogen-filelist
%defattr(-,root,root)

%doc AUTHORS TODO COPYING NEWS NOTES THANKS README VERSION

%changelog
[=

;; Run the following script at spec creation time to insert the
;; "regenerated" change log entry
;;
(out-push-new)

=]
test -z "${LOGNAME}" && {
  LOGNAME=`logname` 2>/dev/null
  case "${LOGNAME}" in
  *"no login name" )
    LOGNAME=`id | sed 's,).*,,;s,^.*(,,'` ;;
  esac
}
name=`grep ^${LOGNAME}: /etc/passwd | \
      sed 's,:/.*,,;s,.*:,,'` 2>/dev/null
date=`date '+%a %b %e %Y'`
domain=`dnsdomainname` 2>/dev/null

echo \* ${date} ${name} \<${LOGNAME}@${domain}\> Regenerated
[=

(shell (out-pop #t))

=]
* Fri Dec 31 2004 Bruce Korb <bkorb@gnu.org> Restored the file list
* Wed Oct 27 2004 Ed Swierk <eswierk@users.sf.net> fixed up for Fedora
* Tue Dec 16 2003 Richard Zidlicky <rz@linux-m68k.org> 5.5.7pre5-5
- fix %%doc
- add post/pre scriptlets
- change default prefix
* Sat Mar 15 2003 Bruce Korb <bkorb@gnu.org>
- Rework as a template to automatically produce a properly configured RPM
* Fri Aug 9 2002 Bruce Korb <bkorb@gnu.org>
- Pull stuff from Thomas Steudten's version of this file[= #'

## Local Variables:
## mode: shell-script
## minor-mode: rpm
## indent-tabs-mode: nil
## End:
## end of spec.tpl =]
