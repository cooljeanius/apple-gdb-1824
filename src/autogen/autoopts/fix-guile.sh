#! /bin/false

scan_args() {
    libguiledir=
    while test $# -gt 0
    do
        case "$1" in
        -I )
            test -f $2/libguile/__scm.h && {
                libguiledir=$2
                return 0
            }
            ;;
        -I* )
            f=${1#-I}
            test -f $f/libguile/__scm.h && {
                libguiledir=$f
                return 0
            }
            ;;
        esac
        shift
    done

    libguiledir=/usr/include
    test -f $libguiledir/libguile/__scm.h && return 0
    echo "The Guile header __scm.h cannot be found" 1>&2
    exit 1
}

find_libguiledir() {
    guile_scm_h=
    libguiledir=`exec 2>/dev/null ; guile-config info includedir`
    test -d "$libguiledir" || scan_args "$@"

    guile_scm_h=${libguiledir}/libguile/__scm.h
}

find_libguiledir "$@"

list=`find ${libguiledir}/libguile* -type f`
list=`grep -l -E '\<noreturn\>' $list`

test -z "$list" && exit 0

test -d libguile || mkdir libguile || {
    echo "cannot make libguile directory"
    exit 1
} 1>&2

noret='\([^a-zA-Z0-9_]\)noreturn\([^a-zA-Z0-9_]\)'
nores='\1__noreturn__\2'
sedex="s@${noret}@${nores}@"

for f in $list
do
    g=libguile${f##*/libguile}
    sed "${sedex}" $f > $g
done

test -f libguile.h || cp ${libguiledir}/libguile.h .
