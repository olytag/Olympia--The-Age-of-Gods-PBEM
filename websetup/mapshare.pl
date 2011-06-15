#!/usr/bin/perl

use strict;

my $mapsize = "0 39 0 39";

my $thisdir = `pwd`;
chomp $thisdir;
my $webdir = `dirname $thisdir`;
chomp $webdir;
my $myfaction = `basename $thisdir`;
chomp $myfaction;
#print "dir = $webdir faction = $myfaction\n";

open FILE,"<mapshare.trusted" or die "failed to open share file\n";
my @data = <FILE>;
close FILE;

if (-s "mapshare.trusted" < 2) {
  exit;
}
my @factions;
my $n = 0;
foreach my $line (@data) {
  chomp $line;
  my @pieces = split / /,$line;
  for (my $i=0;$i<=$#pieces;$i++) {
    if (length($pieces[$i]) > 1) {
      $factions[$n] = $pieces[$i];
      chomp $factions[$n];
      if ($factions[$n] =~ /\W/) { chop $factions[$n]; }
      $n++;
    }
  }
}

# get my list of non-zero-size map files
chdir "info";
my @files = `ls -1 *.html`;
#print @files;

# loop through all target factions
for (my $i = 0; $i < $n; $i++) {
  #print "fac = $webdir/$factions[$i] " . length($factions[$i]) . "\n";

  # only process for real target directories
  if (! -e "$webdir/$factions[$i]/info") { next; }
  if ($factions[$i] eq $myfaction) { next; }

  #print "found $factions[$i]\n";

  # get list of possible target files
  chdir "$webdir/$factions[$i]/info";
  my @targetfile = `ls -1 *.html`;
  foreach my $tf (@targetfile) {
    chomp $tf;
    if ($tf =~ /\W/) { chop $tf; }
  }

  # check to see if the file we are copying from:
  #   a) doesn't exist in the target dir or
  #   b) is tiny (0-length) in the target dir
  # and if either case is met then we copy it
  foreach my $f (@files) {
    chomp $f;
    my $copyme = 0;
    if (! -e $f) {
      #print "COPY $f\n";
      $copyme = 1;
    } else {
      if (-s $f < 2) {
        #print "COPY - SMALL $f\n";
        $copyme = 1;
      } else {
        #print "NO COPY $f\n";
      }
    }
    if ($copyme == 1) {
      # copy the regular info file:
      open FILE,"<$thisdir/info/$f" or die "failed to open $thisdir/info/$f\n";
      my @stuff = <FILE>;
      close FILE;
      open FILE,">$webdir/$factions[$i]/info/$f","664" or die "failed to open $webdir/$factions[$i]/info/$f\n";
      foreach my $line (@stuff) {
        if ($line =~ /h4.Last updated /) {
          print FILE "<b><i>Shared by faction [$myfaction]</i></b>\n";
        }
        print FILE $line;
      }
      close FILE;
      # copy the HTML info file:
      open FILE,"<$thisdir/info_html/$f" or die "failed to open $thisdir/info_html/$f\n";
      my @stuff = <FILE>;
      close FILE;
      open FILE,">$webdir/$factions[$i]/info_html/$f"."664" or die "failed to open $webdir/$factions[$i]/info_html/$f\n";
      foreach my $line (@stuff) {
        if ($line =~ /h4.Last updated /) {
          print FILE "<b><i>Shared by faction [$myfaction]</i></b>\n";
        }
        print FILE $line;
      }
      close FILE;
    }
  }
  chdir "$webdir/$factions[$i]/info";
  `perl $thisdir/map.pl map.html $mapsize full`;
  `cp map.html ../info_html`;
}

