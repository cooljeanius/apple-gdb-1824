#
# The PROTO macro is a subterfuge to be compatible with both ANSI and K&R 
# declaration syntax. It is not widely known, so for the docn just map the 
# thing to ANSI declaration syntax.
# 
# First, join up defns broken across multiple lines in source---but leave
# any linebreaks, to prettify our examples 
:pbegn
/PROTO(.*, *$/N
s/\n/?/
t pbegn
s/?/\
/g
# Now actually do the PROTO interpretation.
# A PROTO invocation looks like
#   PROTO( resulttype, function, (arglist));
s/[ 	]*PROTO(\(.*\),[\n 	]*\(.*\),[\n 	]*\((.*)\));/\1 \2\3;/

