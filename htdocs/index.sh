#!/bin/sh

if [ $# -gt 1 ]
then
    for i in "$@"
    do
	$0 $i
    done
    exit 1
elif [ $# -eq 0 ]
then
    echo "Usage: $0 index.htm ...." 1>&2
    exit 1
fi

if test ! -r $1
then
    echo "Warning: ignoring non-existant $1" 1>&2
    exit 0
fi

if grep -e '-- body' < $1 > /dev/null 2>&1
then
    :
else
    echo "Warning: ignoring non-standard $1" 1>&2
    exit 0
fi

cd `dirname $1`
index=`basename $1`
echo "$1"

cp ${index} index.in
exec > index.out

title=`sed -n -e 's,^.*<title>\(.*\)</title>.*$,\1, p' < index.in`
date=`date +%Y-%m-%d`

# Create a relative prefix
prefix="../"
dir=`/bin/pwd`
while true
do
    case ${dir} in
	*/htdocs ) b=gdb ; break ;;
	*/gdb ) b=gdb ; break ;;
	*/insight ) b=insight ; break ;;
	/ ) exit 1 ;;
    esac
    prefix="../${prefix}"
    dir=`dirname ${dir}`
done
prefix="${prefix}${b}/"

contents ()
{
    cat <<EOF
<center>
<small>
[<a href="${prefix}bugs/">bugs</a>]
[<a href="${prefix}committee/">GDB Maintainers</a>]
[<a href="${prefix}contribute/">contributing</a>]
[<a href="${prefix}current/">current git</a>]
[<a href="${prefix}documentation/">documentation</a>]
[<a href="${prefix}download/">download</a>]
[<a href="${prefix}">home</a>]
[<a href="${prefix}irc/">irc</a>]
[<a href="${prefix}links/">links</a>]
[<a href="${prefix}mailing-lists/">mailing lists</a>]
[<a href="${prefix}news/">news</a>]
[<a href="${prefix}schedule/">schedule</a>]
[<a href="${prefix}song/">song</a>]
[<a href="http://sourceware.org/gdb/wiki/">wiki</a>]
</small>
</center>
EOF
}

hr ()
{
    cat <<EOF
<hr>
EOF
}

cat <<EOF
<?xml version="1.0" encoding="ISO-8859-1"?>
<!DOCTYPE html
          PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
          "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
EOF

cat <<EOF

<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<link rel="shortcut icon" type="image/x-icon" href="${prefix}images/archer-fish.ico" />
<meta name="keywords" content="gdb" />

<title>${title}</title>

</head>

<body bgcolor="#FFFFFF" text="#000000" link="#1F00FF" alink="#FF0000"
vlink="#9900DD">

<a href="${prefix}mascot/"> <img border="0"
src="${prefix}images/archer.jpg" align="right" alt="[image of Archer
Fish]" /></a>

<center><h1>GDB: The GNU Project Debugger</h1></center>

EOF

contents

cat <<EOF

<center><h2>${title}</h2></center>

<!-- NB: CURRENT / LATEST refer to the trunk -->
<!-- NB: MOST RECENT refers to the branch -->

<!-- body, update above using ${prefix}index.sh -->
EOF

cat index.in | ${AWK:-awk} '
BEGIN { state = "head" }
state == "head" && /<!-- body.*-->/ { state = "body"; next }
state == "head" { next }
state == "body" && /<!-- \/body.*-->/ { state = "tail"; next }
state == "tail" { next }
{ $0 =  gensub("(src|href)=\"\\.[\\./]*gdb/", "\\1=\"'${prefix}'", "g") }
{ print }
'

cat <<EOF
<!-- /body, update below using ${prefix}index.sh -->
EOF

echo ""

cat <<EOF
<p></p>
EOF

echo ""

contents

echo ""

cat <<EOF
<hr />
EOF

echo ""

cat <<EOF
<address>

<p>Please send FSF &amp; GNU inquiries &amp; questions to <a
href="mailto:gnu@gnu.org">gnu@gnu.org</a>.  There are also <a
href="http://www.gnu.org/home.html#ContactInfo">other ways to
contact</a> the FSF.</p>

<p>This page is maintained by <a href="${prefix}">the GDB
developers</a>.</p>

<p>Copyright Free Software Foundation, Inc., 51 Franklin St - Fifth
Floor, Boston, MA 02110-1301 USA.</p>

<p>Verbatim copying and distribution of this entire article is
permitted in any medium, provided this notice is preserved.</p>

<p>Last modified ${date}.</p>
</address>
EOF

echo ""

cat <<EOF
</body>
</html>
EOF

cp index.out ${index}
# diff index.in index.out 1>&2
rm index.in index.out
