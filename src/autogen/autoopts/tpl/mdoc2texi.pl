#!/usr/bin/perl

use strict;

my ($optlist,$oldoptlist);
my ($literal);
my ($line);
my ($count,$tableitemcount);

$optlist = 0;       ### 1 = bullet, 2 = enum, 3 = tag, 4 = item
$oldoptlist = 0;

while ($line = <STDIN>)
{
    if ($line !~ /^\./)
    {
        print $line;
        print ".br\n"
            if ($literal);
        next;
    }

    $line =~ s/^\.//;

    next
        if ($line =~ /\\"/);

    $line = ParseMacro($line);
    print($line)
        if (defined $line);
}

sub ParseMacro #line
{
    my ($line) = @_;
    my (@words, $retval,$columnline);

    @words = split(/\s+/, $line);
    $retval = '';

#   print('@words = ', scalar(@words), ': ', join(' ', @words), "\n");

    while ($_ = shift @words)
    {
        if (/^Bl$/)
        {
            if ($words[0] eq '-bullet')
            {
                if (!$optlist)
                {
                    $optlist = 1; #bullet
                    $retval .= "\@itemize \@bullet\n" ;
                    print "$retval";
                    last;
                }
                else
                {
                    $retval .= "\@itemize \@minus\n";
                    print $retval;
                    $oldoptlist = 1;
                    last;
                }
            }
            if ($words[0] eq '-enum')
            {
                if (!$optlist)
                {
                    $optlist = 2; #enum
                    $retval .= "\@enumerate\n" ;
                    print "$retval";
                    last;
                }
                else
                {
                    $retval .= "\@enumerate\n";
                    print $retval;
                    $oldoptlist = 2;
                    last;
                }
            }
            if ($words[0] eq '-tag')
            {
                    $optlist = 3; #tag
                    $retval .= "\@table \@samp\n";
                    print "$retval";
                    last;
            }
            if ($words[0] eq '-column')
            {
                $optlist = 4; #column
                $retval = "\@multitable \@columnfractions ";#\.20 \.20 \.20\n";
                #print $retval;
                $columnline = "\@headitem ";
                #print $retval;
                foreach(@words)
                {
                    if (!/^"./ && !/-column/ && !/indent/ && !/-offset/)
                    {
                        $_ =~ s/\"//g;

                        $retval .= "\.20 ";
                        if (!$count)
                        {
                            $columnline .= $_;
                        }
                        else
                        {
                            $columnline .= " \@tab ".$_;
                        }
                        $count++;
                    }
                }
                print $retval."\n";
                print $columnline;
                last;
            }
        }
        if ($optlist && /^It$/)
        {
            if ($optlist == 3)
            {
                $retval .= "\@item ".$words[0]."\n";
                print $retval;
                last;
            }
            elsif ($optlist == 4 )
            {
                if (!$tableitemcount)
                {
                    $tableitemcount = 1;
                    last;
                }
                else
                {
                    foreach(@words)
                    {
                        if (/^Li$/)
                        {
                            print "\n\@item ";
                            next;
                        }
                        elsif (/^Ta$/)
                        {
                            print "\n\@tab ";
                            next;
                        }
                        else
                        {
                            print $_;
                            next;
                        }
                    }
                    last;
                }
            }
            else
            {
                print "\@item\n";
            }
        }
        if (/^El$/)
        {
            if ($oldoptlist)
            {
                if ($oldoptlist == 1)
                {
                    $oldoptlist = 0;
                    $retval .= "\@end itemize\n";
                    print $retval;
                }
                elsif ($oldoptlist == 2)
                {
                    $oldoptlist = 0;
                    $retval .= "\@end enumerate\n";
                    print $retval;
                }
            }
            else
            {
                if ($optlist == 1)
                {
                    $oldoptlist = 0;
                    $retval .= "\@end itemize\n";
                    print $retval;
                }
                elsif ($optlist == 2)
                {
                    $oldoptlist = 0;
                    $retval .= "\@end enumerate\n";
                    print $retval;
                }
                elsif ($optlist = 4)
                {
                    $count = 0;
                    $columnline = '';
                    $oldoptlist = 0;
                    $optlist = 0;
                    $tableitemcount = 0;
                    $retval .= "\n\@end multitable\n";
                    print $retval;
                }
                $optlist = 0;
            }
        }
    }
}
