# An awk script to determine the type of a file.
/\177ELF\001/ { if (NR == 1) { print "elf32"; exit } }
/\177ELF\002/ { if (NR == 1) { print "elf64"; exit } }
/\114\001/    { if (NR == 1) { print "pecoff"; exit } }
/\144\206/    { if (NR == 1) { print "pecoff"; exit } }
/0?x?feedfacex?0?/    { if (NR == 1) { print "macho32be"; exit } }
/0?x?feedfacfx?0?/    { if (NR == 1) { print "macho64be"; exit } }
/0?x?ecafdeefx?0?/    { if (NR == 1) { print "macho32le"; exit } }
/0?x?fcafdeefx?0?/    { if (NR == 1) { print "macho64le"; exit } }
/0?x?\xfe\xed\xfa\xcex?0?/  { if (NR == 1) { print "macho32be"; exit } }
/0?x?\xfe\xed\xfa\xcfx?0?/  { if (NR == 1) { print "macho64be"; exit } }
/0?x?\xec\xaf\xde\xefx?0?/  { if (NR == 1) { print "macho32le"; exit } }
/0?x?\xfc\xaf\xde\xefx?0?/  { if (NR == 1) { print "macho64le"; exit } }
