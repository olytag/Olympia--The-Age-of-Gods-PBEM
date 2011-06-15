#!/usr/bin/perl

$accountDir = "/home/tagtest/accounting";
$libHome = "/home/tagtest";		#should contain the lib directory

if (! -e $accountDir) { `mkdir -p $accountDir`; }

$numArgs = $#ARGV +1;

if ($numArgs != 8) {
  print "usage:\n" .
        "   addplayer.pl <factionID> <character name> <start city> <faction  name> <email> <player name> <nation> <game#>\n" .
        "e.g.:\n" .
        "   addplayer.pl aa0 \"Bart Simpson\" \"aj9a\" \"The Simpsons\" bart\@someadress.com \"My Name\" \"Imperial Empire\" 1\n" .
        "   addplayer.pl aa0 \"Bart Simpson\" \"\" \"The Simpsons\" bart\@someadress.com \"My Name\" \"Imperial Empire\" 1\n";
  exit;
}

$factionID = $ARGV[0];
$charName = $ARGV[1];
$city = $ARGV[2];
$facName = $ARGV[3];
$email = $ARGV[4];
$playerName = $ARGV[5];
$nation = $ARGV[6];
$gameNum = $ARGV[7];

print "length = " . length($factionID) . " faction = $factionID\n";
if (length($factionID) != 3 || $factionID !~ /^[a-z][a-z][0-9]/) {
  print "faction ID must be two letters and one digit, like aa0\n";
  exit;
}

if (-e "$accountDir/$factionID") {
  print "ID $factionID is already in use.  Please try another.\n";
  exit;
}

open FILE,">$accountDir/.alloc";
print FILE "$factionID";
close FILE;

`tagacct -n -g 2`;

print "New account = ";
print `ls -d $accountDir/$factionID` . "\n";

if (! -e "$accountDir/$factionID") {
  print "error creating account!\n";
  exit;
}
open FILE,">$accountDir/$factionID/Join-tag-$gameNum" or die "failed to open Join file\n";
print FILE "$facName\n$charName\n$playerName\n$email\n$nation\n$city\n";
close FILE;

chdir $libHome;
`oly -a -S`;
