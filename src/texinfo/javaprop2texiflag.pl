#!/usr/bin/env perl
# javaprop2texiflag.pl --- -*- coding: utf-8 -*-
# Copyright 2012, 2023 Vincent Belaïche
#
# Author: Vincent Belaïche <vincentb1@users.sourceforge.net>
# Keywords:
# X-URL: http://www.jpicedt.org/
#
# Ce logiciel est régi par la licence CeCILL soumise au droit français et respectant les principes de
# diffusion des logiciels libres. Vous pouvez utiliser, modifier et/ou redistribuer ce programme sous les
# conditions de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA sur le site
# "http://www.cecill.info".
#
# En contrepartie de l'accessibilité au code source et des droits de copie, de modification et de
# redistribution accordés par cette licence, il n'est offert aux utilisateurs qu'une garantie limitée.  Pour
# les mêmes raisons, seule une responsabilité restreinte pèse sur l'auteur du programme, le titulaire des
# droits patrimoniaux et les concédants successifs.
#
# A cet égard l'attention de l'utilisateur est attirée sur les risques associés au chargement, à
# l'utilisation, à la modification et/ou au développement et à la reproduction du logiciel par l'utilisateur
# étant donné sa spécificité de logiciel libre, qui peut le rendre complexe à manipuler et qui le réserve donc
# à des développeurs et des professionnels avertis possédant des connaissances informatiques approfondies.
# Les utilisateurs sont donc invités à charger et tester l'adéquation du logiciel à leurs besoins dans des
# conditions permettant d'assurer la sécurité de leurs systèmes et ou de leurs données et, plus généralement,
# à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
#
# Le fait que vous puissiez accéder à cet en-tête signifie que vous avez pris connaissance de la licence
# CeCILL, et que vous en avez accepté les termes.
#
## Commentary:

#

## Installation:

## Code:
use strict;
use warnings;
use feature qw(say unicode_strings);
use PerlIO;

my $version='javaprop2texiflag.pl,v 1.2 2012/09/02 11:17:29 vincentb1';
my $linenb = 0;
my $inputfile;
my $verbose;


sub usage
{
	my $retval = shift;
	print "Usage:
	javaprop2texiflag.pl ARGUMENTS LIST

-h, --header ARG    : Add a header ARG to the produced output
-p, --prefix ARG    : Set prefix to ARG, prefix is empty by default
-i, --input  ARG    : Set input to ARG, otherwise it is STDIN
-i, --output ARG    : Set output to ARG, otherwise it is STDIN
-c, --showcomments  : Translate comments into the output
-e, --showemptylines: Translate empty lines into the output
-v, --verbose       : Output some message when done
--version           : show version and exit
--help              : show this message and exit
";
	exit $retval;
}

#
#
sub  jp2texif_unescape
{
	$_ = shift();
	s!\\(n(?{"\n"})|r(?{"\r"})|f(?{"\f"})|t(?{"\t"})|u([0-9A-F]{4})(?{chr hex $2})|(.)(?{$3}))!$^R!g;
	return $_;
}

sub jp2texif_encode
{
	$_ = shift;
	s!(([\@\{\}])(?{'@'."$2"})|\n(?{'@*'}))!$^R!g;
	# Texinfo-fier les espaces de tête pour les rendre significatifs
	if(/\A([ \t]+)(.*)\Z/)
	{
		my $spaceprefix    = $1;
		my $remainder = $2;
		$spaceprefix =~ s!(.)!\@$1!g;
		$_ = $spaceprefix . $remainder;
	}
	# Texinfo-fier les espaces de queue pour les rendre significatifs
	if(/(.+?)([ \t]+)\Z/)
	{
		my $spacepostfix   = $2;
		my $remainder = $1;
		$spacepostfix =~ s!(.)!\@$1!g;
		$_ = $remainder . $spacepostfix;
	}
	return $_
}

my @header = ();
my $prefix         = "";
my $outputfile;
my $showcomments;
my $showemptylines;

my $i = 0;
while($i < @ARGV){
	if($i + 1 < @ARGV)
	{
		if($ARGV[$i] =~ /\A-(i|-input)\Z/)
		{
			$inputfile = $ARGV[$i+1];
			$i = $i +2;
		}
		elsif($ARGV[$i] =~ /\A-(o|-output)\Z/)
		{
			$outputfile = $ARGV[$i+1];
			$i = $i +2;
		}
		elsif($ARGV[$i] =~ /\A-(p|-prefix)\Z/)
		{
			$prefix = $ARGV[$i+1];
			$i = $i +2;
		}
		elsif($ARGV[$i] =~ /\A-(h|-header)\Z/)
		{
			$header[++$#header] = \$ARGV[$i+1];
			$i = $i +2;
		}
		else
		{
			goto ONE_ARG;
		}
	}
	else
	{
	  ONE_ARG:
		{
			if($ARGV[$i] =~ /\A-(c|-showcomments)\Z/)
			{
				$showcomments = 1;
				$i ++;
			}
			elsif($ARGV[$i] =~ /\A-(e|-showemptylines)\Z/)
			{
				$showemptylines = 1;
				$i ++;
			}
			elsif($ARGV[$i] =~ /\A-(v|-verbose)\Z/)
			{
				$verbose = 1;
				$i ++;
			}
			elsif($ARGV[$i] eq "--version")
			{
				print "Version of javaprop2texiflag.pl = $version\n";
				exit 0;
			}
			elsif($ARGV[$i] eq "--help")
			{
				usage(0);
				exit;
			}
			else
			{
				print "Invalid remaining arguments: @ARGV[$i .. $#ARGV]\n";
				usage(-1);
			}
		}
	}
}

my $in;
if($inputfile)
{ 
	open($in,  "< :encoding(ISO-8859-1)", $inputfile)  or die "Can't open $inputfile $!";
}
else
{
	$in = \*STDIN;
}
my $out;
if($outputfile)
{
	open($out, "> :encoding(UTF-8)", $outputfile) or die "Can't open $outputfile $!";
}
else
{
	$out = \*STDOUT;
}
select $out;

my $line;
if(@header)
{
	foreach(@header){
		say '@c ', $$_;
	}
}

my $folded_line = 0;
my $flagnb      = 0;

my $propname;
my $propval;
my $nextpropval;

LINE: while(<$in>){
	$line = $_ ;
	$linenb++ ;
	if($line =~ /\A(\s*)[#!](.*)\Z/)
	{
		if($showcomments)
		{
			say "$1" , '@c ' , "$2";
		}
		next LINE;
	}
	elsif($line =~ /\A\s*\Z/)
	{
		if($showemptylines)
		{
			say "\n";
		}
		next LINE;
	}
	elsif($line =~ /\A(\s*(.*))\Z/)
	{
		if($folded_line == 0)
		{
			if($line =~ /\A\s*((?:[a-zA-Z0-9_\.-]|\\[nr=:])+)\s*[=:](.*)\Z/)
			{
				$propname = $1;
				$propval = $2;
				if($propval =~ m!(\\+)$! && (length($1) & 1) == 1)
				{
					# nombre impair de contre-obliques en fin de ligne, c'est un repliement
					$folded_line = 1;
					$propval =~ s!.$!!;
					$propval = jp2texif_unescape($propval);

				}
				else
				{
					say "\@set $prefix$propname " , jp2texif_encode( jp2texif_unescape($propval));
					$flagnb ++;
				}
			}
			else
			{
				die "$inputfile:$linenb: Invalid line = $line";
			}
		}
		elsif($folded_line == 1)
		{
			$nextpropval = $2;
			if($nextpropval =~ m!(\\+)$! && (length($1) & 1) == 1)
			{
				# nombre impair de contre-obliques en fin de ligne, on reste en repliement
				$nextpropval =~ s!.$!!;
				$propval = $propval . jp2texif_unescape($nextpropval);
			}
			else
			{
				# le repliement est fini
				$folded_line = 0;
				$propval = $propval . jp2texif_unescape($nextpropval);
				say "\@set $prefix$propname " , jp2texif_encode($propval);
				$flagnb ++;
			}

		}
	    else
		{
			die "$inputfile:$linenb: javaprop2texiflag INTERNAL BUG";
		}

		next LINE;
	}
	else
	{
		die "$inputfile:$linenb: Invalid line = $line";
	}
}

if($verbose)
{
	if($inputfile)
	{
		$inputfile = "file \`$inputfile\'";
	}
	else
	{
		$inputfile = "standard input";
	}
	print STDOUT "\nDone: javaprop2texiflag is finished,\n\tinput was $inputfile,\n\t$linenb lines were processed,\n\t$flagnb flags were produced.\n";
}
