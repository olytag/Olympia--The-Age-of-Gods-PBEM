#!/usr/bin/perl

$home = "/home/tagtest";

#make a list of characters and their email addresses so we can create
#a relay from one character to another.
#
#this should only work from game email addresses, for factions and for
#player-characters
#
#     Internal ID                  Type
# *  1-9999             9999     reserved (items, skills)
# *
# *  10,000-19,999    10,000     provinces               (CCNN: AA00-DV99)
# *  20,000-26,759      6760     player entities         (CCN)
# *  26,760-33,519      6760     lucky characters        (CNC)
# *  33,520-36,119      2600     lucky locs              (CNN) 
# *  36,120-102,400   66,279     sublocs, runoff         (CCNC)

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
  $email[$n] =~ s/ /,/g,$email[$n]; # replace space in email list with comma
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
  # usually former refugees fall into this category
  {
    $id -= 36120;
    $a = $id % 26;
    $id /= 26;
    $id = int $id;
    $b = $id % 10;
    $id /= 10;
    $id = int $id;
    $c = $id % 26;
    $id /= 26;
    $id = int $id;
    $plate = $letters[$id] . $letters[$c] . $b . $letters[$a];
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
#        if (l < 36120)                  /* CNN */
#        {
#                l -= 33520;
#
#                n = l % 10;
#                l /= 10;
#
#                a = l % 10;
#                b = l / 10;
#
#                return sout("%c%d%d", letters[b], a, n);
#        }
#
#        {                               /* CCNC */
#                l -= 36120;
#
#                a = l % 26;
#                l /= 26;
#
#                b = l % 10;
#                l /= 10;
#
#                c = l % 26;
#                l /= 26;
#
#                return sout("%c%c%d%c", letters[l], letters[c], b, letters[a]);
#        }

