#! /bin/sh

sedcmd=
nl='
'
eval `egrep '^SED=' ${top_builddir}/config/shdefs`

for v in srcdir top_srcdir top_builddir
do
    eval val=\"\${$v}\"
    val=`cd ${val} >/dev/null && pwd`
    sedcmd="${sedcmd}/: .{$v=/s@=[^}]*}@=$val}@$nl"
done

${SED} "$sedcmd" $1 > ./.defs$$
mv .defs$$ defs
chmod +x ${srcdir}/*.test >/dev/null 2>&1
