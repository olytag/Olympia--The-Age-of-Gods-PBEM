#!/usr/bin/perl

open FILE,"<loc" or die "failed\n";

@data = <FILE>;
close FILE;

$xsize=40;
$ysize=40;

$incity = 0;
foreach $i (@data) {
  if (substr($i,0,1) eq "1") {
    $y = substr($i,1,2);
    $x = substr($i,3,2);
    if ($i =~ /loc ocean/) { $terrain = "."; }
    if ($i =~ /loc forest/) { $terrain = "f"; }
    if ($i =~ /loc mountain/) { $terrain = "M"; }
    if ($i =~ /loc swamp/) { $terrain = "s"; }
    if ($i =~ /loc plain/) { $terrain = "p"; }
    if ($i =~ /loc desert/) { $terrain = "D"; }
    $type[$x][$y] = $terrain;    
  }
  if ($i =~ /loc city/) {
    $incity = 1;
  }
  if ($incity && $i =~ / wh /) {
    $y = substr($i,5,2);
    $x = substr($i,7,2);
    $type[$x][$y] .= "*";
    $incity = 0;
  }
}

for ($j=0; $j<$ysize;$j++) {
  for ($i=0;$i<$xsize;$i++) {
    print $type[$i][$j] . ",";
  }
  print "\n";
}
#13939 loc ocean
#LI
# wh 58761
#LO
# pd 13839 13900 10039 13938
#
#57482 loc city
#na Areth Pirn
#LI
# wh 10311
#LO
#SL
# mc 1
#
#58447 loc city
#na Alaris
#LI
# wh 10520
#LO
#SL
# mc 1
##
