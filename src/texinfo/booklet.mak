# -*- makefile -*-
#
# booklet.mak - making booklets from Texinfo.
#
# Copyright 2013 John Darrington.
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# This makefile can be used to generate booklets from Texinfo sources.
# It reduces each page to A5 size and renders two pages per sheet onto
# A4 landscape.  The pages are ordered into a "quire" or "gathering".
# This means that you can print the result on a standard laser printer,
# fold in half and using simple stapler bind the result into a 
# booklet.

# Targets are <foo>-{a4,a5}-book.{dvi,ps,pdf} where <foo>.texi is the texinfo
# source file.
# It is suitable for Texinfo documents up to approx. 70 pages.

# The *-a4-book targets produce A4 half area reduced pages.  The *-a5-book
# targets produce A5 full sized pages.  The *a5-book option will therefore
# have larger text, but will have a higher page count.  It may also have
# problems if the document has @display or @example environments with 
# long lines.

# Example of use:
# 1. make -f Makebook manual-a4-book.ps
# 2. Print the result on a double sided laser printer.  Alternatively
#  if your laser printer does not support double sided printing, print the ODD
#  numbered sheets, retreive the result from the printer and thinking carefully
#  about the page orientation, shove them back into the paper tray.  Depending
#  on your printer, you may have to reverse the order of the sheets.  Then print
#  the EVEN numbered sheets.  I find gv usefull for this.
# 3. Fold the result along the short dimension.
# 4. Staple in place, using a long arm stapler.
# 5. Using a guillotine cut the pages such that their edges co-incide.
# 6. Sit back in an armchair and enjoy your reading.

# For those of you who live in countries which refuse to conform to ISO 216,
# you have a problem.

# End of instructions.

all:
	echo 'Usage: make -f Makebook <target>'


# Mutate the source, with appropriate headings, overriding anything that the 
# author has thoughtlessly imposed.
# We want to specify the paper size, and double headings.  Anything else?
%-a4.texi: %.texi
	sed -e 's/@afivepaper/@afourpaper/' -e '/@end titlepage/a @headings double' $< > $@

%-a5.texi: %.texi
	sed -e 's/@afourpaper/@afivepaper/' -e '/@end titlepage/a @headings double' $< > $@

# Older versions of Texinfo break if the locale is non-english.
%.dvi: %.texi
	LC_ALL=C texi2dvi $< -o $@


# Reorder the dvi into the correct order for the quire.
%-sig.dvi: %.dvi
	dvibook	$< -o $@

# Render 2 pages per sheet, ensuring there is an appropriate "gutter"
%-a5-book.dvi: %-a5-sig.dvi
	dvitodvi  '2:0+1(148mm,0)' $< $@

%-a4-book.dvi: %-a4-sig.dvi
	dvitodvi  '2:700@0(-15mm,-5mm)+1(210mm,-5mm)' $< $@

# print the result in landscape orientation
%.ps: %.dvi
	dvips -t a4 -t landscape -t landscape $< -o $@

%.pdf: %.ps
	ps2pdf $< $@


.PHONY: clean
clean:
	$(RM) *-a[54].*
