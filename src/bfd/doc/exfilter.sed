# SED script for preprocessing embedded doc from source
# (S. Chamberlain markup)
# Final pass; cleanup work is done here.
#
# Within examples, make '{' and '}' printable:
/^@lisp$/,/^@end lisp$/s/{/@{/
/^@lisp$/,/^@end lisp$/s/}/@}/
/^@example$/,/^@end example$/s/{/@{/
/^@example$/,/^@end example$/s/}/@}/
#
# Delete empty @findex and @subsubsection entries (resulting from *proto*
# with no further text on same line, in middle pass)
/^@findex $/d
/^@subsubsection @code{}/d
#
