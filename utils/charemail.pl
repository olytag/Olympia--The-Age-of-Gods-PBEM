#!/usr/bin/perl

$home = "/home/tagtest";

#make a list of characters and their email addresses so we can create
#a relay from one character to another.
#
#this should only work from game email addresses, for factions and for
#player-characters
#
#     Internal ID                  Type
# *  20,000-26,759      6760     player entities         (CCN)
# *  26,760-33,519      6760     lucky characters        (CNC)
#
# master:
#   20002	1.49	fact/20002	Faction Three
#

chdir ("$home/lib/fact");
$n=0;

# get the email addresses for each faction from the "fact" directory

foreach $i (`ls -1 [2-3][0-9][0-9][0-9][0-9]*`) {
  @stuff = `grep " em " $i`;
  $email[$n] = substr($stuff[0],4);
  chomp $email[$n];
  @stuff2 = `grep " pw " $i`;
  if ($#stuff >= 0) {
    $pwd[$n] = substr($stuff2[0],4);
    chomp $pwd[$n];
  } else { $pwd[$n] = ""; }
  $faction[$n] = $i;
  chomp $faction[$n];
  $n++;
}
chdir ($home);
@data = `grep fact\/[2-3][0-9][0-9][0-9] $home/lib/master`;

#print "#ID Char Fact Email\n";

# now check each character in the "fact" file for each faction, and assign them
# an email address

foreach $line (@data) {
  for ($i=0;$i<=$#email;$i++) {
    if ($line =~ /fact\/$faction[$i]/ ) {
      $character = substr($line,0,5);
      ($junk,$junk,$junk,$charname) = split /\t/,$line;
      chomp $charname;
      print licenseplate($character) . "\t" . $character . "\t" .  $faction[$i] . "\t" . $email[$i] . "\t" . $charname . "\t" . $pwd[$i] . "\n";
    }
  }
}

sub licenseplate
{
  my($id) = @_;
  my @letters = ("a","b","c","d","e",
                 "f","g","h","i","j",
                 "k","l","m","n","o",
                 "p","q","r","s","t",
                 "u","v","w","x","y","z");

  if ($id < 26760) { 
    $id -= 20000;
    $n = $id % 10;
    $id /= 10;
    $id = int $id;
    $a = $id % 26;
    $b = $id / 26;
    $plate = $letters[$b] . $letters[$a] . $n;
    return $plate;
  }
  if ($id < 33520) {
    $id -= 26760;
    $n = $id % 26;
    $id /= 26;
    $id = int $id;
    $a = $id % 10;
    $b = $id / 10;
    $plate = $letters[$b] . $a . $letters[$n];
    return $plate;
  }
}
#        if (l < 33520)                  /* CNC */
#        {
#                l -= 26760;
#
#                n = l % 26;
#                l /= 26;
#
#                a = l % 10;
#                b = l / 10;
#
#                return sout("%c%d%c", letters[b], a, letters[n]);
#        }
#
