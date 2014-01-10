#! /bin/sh

die() {
    echo "autofsm test error:  $*" >&2
    exit 1
}

case "$1" in
bootstrap)  testshar=false VERBOSE=true ;;
testing)    testshar=true ;;
*)
     echo USAGE: $0 { bootstrap | testing } >&2
     exit 1
     ;;
esac

exec 8>&1
case "${VERBOSE}" in
"" | no )
  exec >/dev/null 2>&1
  CLEANUP=true
  ;;

* )
  CLEANUP=false
  ;;
esac

rm -f *-*-fsm.[ch] [cn]*-*.def
DEF=${DEF:-example.def}
srcdir=${srcdir:-.}

echo "Constructing source files" >&8
for t in looping stepping reentrant
do for m in callout case ""
  do
    b=${m:-null}-$t
    ( echo "AutoGen Definitions fsm;"
      echo "method = '$m';"
      echo "type   = '$t';"
      sed '/^\(AutoGen\|method \|type \)/d' ${DEF}
    ) > ${b}.def
    autogen -L${srcdir} ${b}.def || exit 1
    test -f ${b}-fsm.c && $testshar && {
      ${CC-cc} -c ${b}-fsm.c || exit 1
      ${CLEANUP} && {
          rm ${b}-fsm.o || exit 1
          echo "${b} built and compiled" >&8
      }
    }
    ls -l ${b}-fsm.*
    ${CLEANUP} && rm -f ${b}.def
  done
done

echo "Construction and compilation successful" >&8

if ${testshar}
then
  echo "Comparing against previously generated versions" >&8

  ${CLEANUP} && rm -f *.samp
  /bin/sh ${srcdir}/sample.shar

  for f in *-fsm.[ch]
  do
    sed -i '/EDIT THIS FILE/,/and the template file/d' $f
    set -- ${f} ${f}.*
    test  -f "${2}" || die "No sample file for ${f}"

    if cmp ${1} ${2}
    then echo "SAME: ${f} ${2}" && ${CLEANUP} && rm -f ${f}*
    else echo "DIFFER: " ${f} ${2} && exit 1
    fi
  done
  echo "source files are unchanged" >&8

else
  echo "Saving results for future comparison" >&8
  rm -f *.samp
  for f in *-fsm.[ch]
  do
    sed '/EDIT THIS FILE/,/and the template file/d' $f > ${f}.samp
  done
  rm -f sample.shar
  shar -o sample.shar *.samp
  [ ! -f sample.shar ] && mv sample.shar.* sample.shar
  rm -f *.samp
  echo sample.shar reconstructed >&8
fi
