#!/usr/bin/perl
#
# splitreport.pl -- version 1.0  31 Aug 2000
#
# This program scans a TAG [TEXT] format file and splits out the
# following information into separate HTML files:
#
#      - province reports
#      - city reports
#      - garrison inventories
#      - guild reports
#      - ship reports
#
# These files are written to "info/ID.html", where ID = (the ID of
# the item being saved).  Needless to say, you will need to create
# a subdirectory called "info" in order for this to work.
#
# This script looks through all the lines of the turn reports, logging
# the beginning and ending of each section, and then writes it all to
# seprate files.

#
# Start the program, read reports:
#

$numArgs = $#ARGV + 1;
if ($numArgs != 2 && $numArgs != 3) {
  print "Usage:  splitreport.pl <turn report file> <target dir> <optional html flag=1>\n";
  print "\n";
  exit;
}    
$FILE = $ARGV[0];
open FILE or die "failed to open $FILE!";

@data = <FILE>;
close(FILE);

$target_dir = $ARGV[1];
if (! -e $target_dir) { `mkdir -p $target_dir`; }

$html_flag = $ARGV[2];	#1 turns it on
#
# find the turn number and faction
#

foreach $line (@data) {
  if ($line =~ /TAG.game.(.*).turn.(.*).report/i) {
    $turn = $2;
    $game = $1;
    if ($line=~ /\[([^\]]*)\]/) {
        $faction= $1;
    }
    print "Turn info = " . $turn . " " . $game . " " . $faction . "\n";
    print $line;
  }
}

#
#initialize some variables
#

$endline = 0;
$mestext = "";
$count = 0;
$locs = 0;
$garrstart = 0;
$garrstart2 = 0;
$garrend = 0;
$garrend2 = 0;

#
#loop through each line of the turn report.  If we find a section break
#line, then signal to start processing data
#

foreach $line (@data) {
  $_ = $line;
  if (/------------------------------------------------------------------------/) {
    if ($garrend == 0) { $garrend = $count-2}	#end of garrison reports
    if ($garrstart2 > 0 && $count > ($garrstart2+2) && $garrend2 == 0) {
      $garrend2 = $count-2;
    }
    $inner = 0;					#init for inner hidden locs
    @fields = split /,/,$data[$count-1];	#one line before section
    @fields2 = split /,/,$data[$count-2];	#two lines before section,
						#since some titles are
						#two lines long
    $market[$locs] = 0;
    $other[$locs] = 0;
    if ($fields[1] eq " desert" || $fields[1] eq " plain" || 
	$fields[1] eq " mountain" || $fields[1] eq " city" ||
	$fields[1] eq " port city" || $fields[1] eq " forest" ||
	$fields[1] eq " ocean" || $fields[1] eq " swamp" ||
	$fields[1] eq " mine-shaft" || $fields[1] eq " Hades" ||
	$fields[1] eq " ship" || $fields[1] eq " cloud" ||
	$fields[1] eq " graveyard" || $fields[1] eq " underground" ||
        $fields[1] eq " faery hill" || $fields[2] eq " mine-shaft"  ||
        $fields[1] eq " Trading Guild" || 
	$fields[1] eq " Shipcraft Guild" ||
	$fields[1] eq " Combat Guild" || 
	$fields[1] eq " Stealth Guild" || 
	$fields[1] eq " Persuasion Guild" ||
	$fields[1] eq " Construction Guild" || 
	$fields[1] eq " Alchemy Guild" || 
	$fields[1] eq " Forestry Guild" || 
	$fields[1] eq " Mining Guild" || 
	$fields[1] eq " Ranger Guild" ||
        $fields[1] eq " Heroism Guild")
    {
      if ($data[$count-1] =~ /\[([^\]]*)\]/) {
        $locid[$locs] = $1;
        $locations[$locs] = $data[$count-1];
        $locpos[$locs] = $count - 1;
        if ($fields[1] eq " port city" || $fields[1] eq " city") {
  	  $market[$locs] = 1;
        }
	if ($fields[1] eq " ship") {
	  $other[$locs] = 1;
	}
	$locfull[$locs] = 0;	#is this a full report?
        $locs++;
      }
    }
    if ($fields2[1] eq " Trading Guild" || 
	$fields2[1] eq " Shipcraft Guild" ||
	$fields2[1] eq " Combat Guild" || 
	$fields2[1] eq " Stealth Guild" || 
	$fields2[1] eq " Persuasion Guild" ||
	$fields2[1] eq " Construction Guild" || 
	$fields2[1] eq " Alchemy Guild" || 
	$fields2[1] eq " Forestry Guild" || 
	$fields2[1] eq " Mining Guild" || 
	$fields2[1] eq " Ranger Guild" ||
        $fields2[1] eq " Heroism Guild" || 
	$fields2[1] eq " graveyard" ||
	$fields2[1] eq " ship" ||
        $fields2[2] eq " mine-shaft") {
      if ($data[$count-2] =~ /\[([^\]]*)\]/) {
        $locid[$locs] = $1;
        $locations[$locs] = $data[$count-2];
        $locpos[$locs] = $count - 2;
	if ($fields2[1] eq " Trading Guild") {
	  $market[$locs] = 2;
	}
	if ($fields2[1] eq " ship") {
	  $other[$locs] = 1;
	}
        $locs++;
      }
    }

  }
#
# check to see if it's the full report for this province, or just a report
# we get due to garrisons that doesn't have the "who's here" info
#
  if ((/Routes leaving/ || /Province controlled by/ || /Ocean routes/ || /No known routes leaving/) && $locs > 0) {
    $locfull[$locs-1] = 1;
    $locroute[$locs-1] = $count -1;	#end of log
print "Full = ",$locid[$locs-1],"\n";

  }

#
# check to see if there was a scry, vision, etc.
#
  if (/A vision of/ || /receives a vision of/) {
#not done yet...
  }

#
#check for hidden locations and log them to their own file
#  (note that there is a bug, here, that trips if "hidden" is on
#   a different line than the location ID)
#
  if (/Inner locations/) { $inner = 1; }
  if ($inner > 0) {
    if (/ hidden, /) {
      $HFILE = ">>$target_dir/hidden.dat";
      open HFILE or die "open failed for $HFILE!";
      print HFILE $locid[$locs-1],"\n";
      print HFILE $data[$count];
      close (HFILE);
    }
  }
  
#
#Check for garrison reports
#
  if (/garr where  men cost/ || /garr where border  men cost/) {
    $garrstart = $count;
  }
  if (/Garrison log/) {
    $garrstart2 = $count;
  }

# 
#Check for the last line of data
#
  if ($endline == 0 && ($line =~ /Order template/ || $line =~ /Lore sheet/ || $line =~ /New players/) ) {
    print $line;
    $endline = $count;
    $locpos[$locs] = $count;
  }

  $count++;
}
#print $count,"\n";

for ($i=0;$i<$locs;$i++) {
  print $locpos[$i],"--", $locfull[$i],"--",$locations[$i];
}

#
# print output files
#
$j = 0;
$printfile = 0;
for ($i=$locpos[0];$i<$endline;$i++) {
  if ($i == $locpos[$j] && $locfull[$j] == 1) {
    $printfile = 1;
    $html_on = 0;
    $FILE = ">$target_dir/" . $locid[$j] . ".html";
    open FILE or die "open failed for $FILE!";
    $LISTFILE = ">>$target_dir/info.tmp";
    open LISTFILE or die "couldn't open list file\n";
    print LISTFILE $locid[$j]," ",$faction,"\n";
    close (LISTFILE);
    print $locpos[$j],"--",$locid[$j];
    print "\n    ",$data[$i];
    print FILE "<html><head><title>",$locations[$j],"</title></head>\n";
    print FILE "<body>\n";
    print FILE "<!--\n--TURN#",$turn," ",$locations[$j],"\n-->\n";
    if ($market[$j] >= 1) {
      print FILE "<!--\n--MARKET: T#",$turn," ",$locations[$j],"\n-->\n";
    }
    if ($market[$j] == 2) {
      print FILE "<!--\n--TRADEGUILD: T#",$turn," ",$locations[$j],"\n-->\n";
    }
    if ($other[$j] == 1) {
      print FILE "<!--\n--SHIP: T#",$turn," ",$locations[$j],"\n-->\n";
    }
    print FILE "<hr><h2>",$locations[$j],"</h2>\n";
    print FILE "<h4>Last updated Turn #",$turn,"</h4><pre>\n";
  }
  if ($i == $locpos[$j]) { $j++ }
  if ($printfile == 1) {
    if ($data[$i] =~ /\[(.{3,4})\].*\[(.{3,4})\]/) {
      $a1 = $1; $a2 = $2;
      $b1 = "<a href=\"$a1.html\">$a1</a>";
      $b2 = "<a href=\"$a2.html\">$a2</a>";
      if ($html_flag && $html_on && substr($data[$i],0,6) ne "      ")  {
        $data[$i] =~ s/$a1/$b1/g;
        $data[$i] =~ s/$a2/$b2/g;
      }
      print FILE $data[$i];
    } elsif ($data[$i] =~ /\[(.{3,4})\]/) {
      $a1 = $1;
      $b1 = "<a href=\"$a1.html\">$a1</a>";
      if ($html_flag && $html_on && substr($data[$i],0,6) ne "      ")  {
        $data[$i] =~ s/$a1/$b1/g;
      }
      print FILE $data[$i];
    } else {
      $_ = $data[$i];
      if (/Routes leaving/ || /Inner locations/ || /Seen here/ || /Cities rumored/ || /Skills taught here/ || /Market report/) {
        print FILE "<B>",$data[$i],"</b>";
      } else {
        if (/-----------------------------------------------/) {print "\n";}
        print FILE $data[$i];
      }
      if (/Routes leaving/ || /Inner locations/ || /Cities rumored to be nearby/) {
        $html_on = 1;
      }
      if (/Seen here/ || /Skills taught here/ || /Market report/) {
        $html_on = 0;
      }
    }
  }
  if ($i == ($locpos[$j]-1) && $printfile == 1) {
#    print FILE "</pre></body></html>\n";
    close (FILE);
    $printfile = 0;
  }

}

#
#print out garrison file
#
if ($garrstart != 0 && $garrend != 0) {
  $FILE = ">>$target_dir/garrison.html";
  open FILE or die "open failed for $FILE!";
#print FILE "<html><head><title>Garrison report</title></head>\n";
#print FILE "<body>\n";
  print FILE "<hr><h2>Garrison Report for [",$faction,"]</h2>\n";
  print FILE "<h4>Last updated Turn #",$turn,"</h4><pre>\n";
  print FILE "<pre>";
  for ($i=$garrstart;$i<$garrend;$i++) {
    print FILE $data[$i];
  }
  for ($i=$garrstart2;$i<$garrend2;$i++) {
    print FILE $data[$i];
  }
  print FILE "</pre>\n";
}

