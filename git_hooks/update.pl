#! /usr/bin/perl -w

# Hook for checking the log format given to git.
# Written by Thomas "Enki" Badie <badie@lrde.epita.fr> and
# Alexandre Duret-Lutz <adl@lrde.epita.fr>

# This hook rejects push containing invalid commit messages, i.e.,
# messages that we will not be able to convert into a GNU-style ChangeLog.

# To add a check, find a spot in "check_log_style", and add your own
# chunk.  You have to add your errors to @errors without trailing
# newline.

use strict;
use File::Basename;

# The classical argv[0], without the relative path.
my $ME = basename $0;

# The list of ignored files... Add your own here.
my %IGNORED_FILES = map { $_ => 1 } qw(
  ChangeLog .gitignore NEWS THANKS lrde-upload.sh
);

# Internal global.
my $nb_error = 0;
my $warning = 0;

sub print_usage()
{
  print "usage:\n$ME <ref> <oldrev> <newrev>
$ME <path/to/commit/message>\n";
}

# Get the files listed in a commit message.
sub file_list_from_message ($)
{
    my ($log) = @_;

    return () if $log !~ /^\s*\*\s/m;

    # Remove anything up to the first leading star.
    $log =~ s/^.*?\n\s*\*//ms;

    # Remove any parenthesized and bracketed blocks.
    # First remove one-level nested parenthesizes,
    # then remove one-level parenthesizes and brackets.
    $log =~ s/\((?:[^()]*\([^)]*\)[^()]*)*\)//g;
    $log =~ s/\([^)]+\)//g;
    $log =~ s/\[[^]]+\]//g;

    # Remove description of changes.
    $log =~ s/:([^\n]|\n(?=[^*]))*//g;
    # Remove leading stars.
    $log =~ s/^\s*\*\s*//mg;

    # Remove extra lines at beginning or end.
    $log =~ s/^[\s,]+//s;
    $log =~ s/[\s,]+$//s;

    return split '[\s,]+', $log;
}


## The function which contains all the style check.
## String which is the concatenation of all the file.
## Receive the log message in raw.
## The SHA1 of the commit.
sub check_log_style($$$)
{
  my ($list, $message, $sha) = @_;

  my @modified_files;  # non-ignored modified files
  my %modified_files;  # ALL modified files

  # A commit message that starts with [name] contains
  # paths relative to the name/ subdirectory.
  my $subdir = '';
  if ($message =~ m|^\[([\w/]+)\]|s)
  {
      $subdir = "$1/";
  }

  foreach (split '\n', $list)
  {
    next unless $_;
    s|^$subdir||;
    my $long_name = $_;
    s|.*/||;

    # If a/b/d.c is modified, also mark a/b/ and a/ as modified.
    # This way we can deal with commit messages sush as:
    #  * a/b/: Rename as ...
    #  * ab/: ... this.
    my $file = $long_name;
    while ($file ne "./")
    {
	$modified_files{$file} = 1;
	$file = dirname($file) . "/";
    }

    next if defined $IGNORED_FILES{$_};
    push @modified_files, $long_name;
  }

  # Do not check commits that touch only ignored files.
  return 0 unless @modified_files ;

  # Gather all files mentioned in the ChangeLog
  my @listed_files = file_list_from_message($message);
  my %listed_files = map { $_ => 1 } @listed_files;

  # Will contain all error messages.
  my @error;


  # Check if the star before a file list is on the first column.
  # (($` =~ tr/\n/\n/) + 2) counts the number of line. Generally "+1"
  # is correct, but because we are using $` it is the line before the
  # matching.
  push @error, "The star must be on the first column."
      if $message =~ /^[ \t]+\*\s/m;


  # Make sure all listed files are modified and vice-versa.
  for (@listed_files)
  {
      next unless $_;
      next if defined $IGNORED_FILES{$_};
      next if defined $modified_files{$_};

      push @error, "`$_' is mentioned in message but not modified by commit.";

  }
  MFILE: for (@modified_files)
  {
      my $file = $_;
      while ($file ne "./")
      {
	  next MFILE if defined $listed_files{$file};
	  # Strip filename in case the commit message states it is
	  # renaming a directory.
	  $file = dirname($file) . "/";
      }

      push @error, "`$_' is modified by commit but not mentioned in message.";
  }

  # Any commit of more than one line should have a one-line summary.
  # We reuse the trick used in vc-dwim: if the first line starts with
  # a star, it means there is no one-line summary.
  my @lines = split(/\n/, $message);
  push @error, "Missing one-line summary."
    if ($#lines > 0) && ($message =~ /^[\s\n]*\* /);


  # If there is a one-line summary, we want it to fit on one line.
  push @error, "One-line summary must fit on the first line."
    if ($#lines > 0) && ($message !~ /^[\s\n]*\* /)
    && ($lines[1] !~ /^(?:[\s\n]*\*.*)?$/);


  push @error, "No empty lines between two files."
      if ($message =~ /^\s*\*.*\n\n/m);

  # The message should have at least one line of the form
  # "* file: ..."
  push @error, "No file found in commit message."
      if ($message !~ /^\s*\* [^*:]*:/ms);

  # We should always find a `:' between two entries.
  push @error, "Missing `:' between two entries."
      if ($message =~ /^\* [^*:]*\n\*/m);

  push @error, "Line exceeds 72 characters."
      if ($message =~ /^.{73,}$/m);


  push @error, "No space before colon in English."
      if ($message =~ /\s:/);

  # SPOT !
  push @error, "accepting -> acceptance."
      if ($message =~ /\baccepting\s+cond/im);


  # Print any error resulting from the above checks.
  if (@error)
  {
      my $diag = $warning ? "WARNING" : "ERROR";

      print STDERR "$diag: In commit $sha:\n" if defined $sha;
      $message =~ s/^/| /gm;
      print STDERR $message;

      foreach (@error)
      {
	  print STDERR "* $_\n";
      }

      $nb_error += @error;
  }
}


# The main get the list of commit and call the checker for each
# commit.
sub main_server
{
  # Get the information from the command line.
  my $refname = $ARGV[0];
  my $oldrev = $ARGV[1];
  my $newrev = $ARGV[2];

  unless (defined $refname
          && defined $oldrev
          && defined $newrev)
  {
    print_usage;
    exit 1;
  }

  print "Running hook: Log checker.\n";

  # 80 columns :)
  my $zero="0000000000000000000000000000000000000000";

  # It is a branch deletion. Nothing to say about this.
  if ($newrev eq $zero)
  {
    return 0;
  }

  # We want the name of the branch.
  (my $branch = $refname) =~ s+refs/heads/++;


  # Display warnings only (no error) on branches whose name contain a
  # "/", except for "candidates/*".
  $warning = 1 if ($branch =~ m#/#)
      && ($branch !~ m#^candidates/#);

  my $revspec = undef;

  $revspec = "$newrev" if $oldrev eq $zero;
  $revspec = "$oldrev..$newrev" if not defined $revspec;

  my $other_branches =
      `git for-each-ref --format='%(refname)' refs/heads |
        grep -F -x -v $refname |
        grep -x 'refs/heads/\\(candidates/.*\\|[^/]*\\)'`;

  # $other_branches is a new-line-separated list of branches.
  # We want it space-separated.
  $other_branches =~ y/\n/ /;

  my $new_revisions=`git rev-parse --not $other_branches |
    git rev-list --stdin $revspec`;

  my @commits = split '\n', $new_revisions;

  foreach (reverse @commits)
  {
    if ($warning == 0 && $nb_error >= 20)
    {
	print STDERR
            "Too many errors in commit messages."
            . " Fix the above and try again.\n";
	exit 1;
    }

    chomp;

    my $modified_files = `git diff-tree --name-only -r --no-commit-id $_`;
    my $commit_log = `git log -1 --pretty=format:%B $_`;

    # Apply the checks on the commit.
    check_log_style($modified_files, $commit_log, $_);
  }
}

sub main_commit()
{
  my $file = $ARGV[0];

  open my $fh, "< $file";

  local $/ = undef; # Turn on slurp mode.
  my $message = <$fh>;

  # Remove all comment lines.
  $message =~ s/^#[^\n]*\n//mg;

  my $modified_files;

  foreach (split /\n/, `git status --porcelain`)
  {
    next if /^ / or /^\?/;

    my @Fld = split / /, $_;

    $modified_files .= $Fld[$#Fld] . "\n";

    if (/->/)
    {
      $modified_files .= $Fld[2] . "\n";
    }
  }

  check_log_style($modified_files, $message, undef);
}


if (@ARGV == 1)
{
  main_commit();
}
else
{
  main_server();
}
exit 1 if $nb_error && $warning == 0;

