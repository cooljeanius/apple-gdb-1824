#! /bin/bash

cd ${addon_dir}/${dir}

case "${1}" in
( description )
    text=$(sed -n $'1,/^DESCRIPTION:/d;/^PURPOSE:/q;p' \
        editor.tpl)
    title=$(echo "${text}" | head -n 1)
    text=$(echo "${text}" | sed 1d)
    printf '<h3><a href="#%s">%s</a></h3><p>%s</p>' ${dir} "${title}" "${text}"
    ;;

( files )
    make >&4 2>&4
    primary_source='editor.tpl editor.def'
    example_source='daemon.c daemon-opts.def'
    example_output=$(ls -1 *-conf.[ch])
    build_example=
    text=$(
        sedcmd=$'1,/^PURPOSE:/d
	    /^[A-Z ]\\+:$/q
	    /^[ \t]*$/d
	    p'
        sed -n "$sedcmd" editor.tpl)
    ;;
esac
