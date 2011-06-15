#!/usr/bin/perl

$numArgs = $#ARGV + 1;
if ($numArgs != 6) {
  print "Usage:  map.pl <mapfile> <xmin> <xmax> <ymin> <ymax> <full/small/nohtml>\n";
  print "\n";
  exit;
}
@x = (  "00","01","02","03","04","05","06","07","08","09",
	"10","11","12","13","14","15","16","17","18","19",
	"20","21","22","23","24","25","26","27","28","29",
	"30","31","32","33","34","35","36","37","38","39",
	"40","41","42","43","44","45","46","47","48","49",
	"50","51","52","53","54","55","56","57","58","59",
	"60","61","62","63","64","65","66","67","68","69",
	"70","71","72","73","74","75","76","77","78","79",
	"80","81","82","83","84","85","86","87","88","89",
	"90","91","92","93","94","95","96","97","98","99",
	"100");

@y = (  "aa","ab","ac","ad","ae","af","ag","ah","ai","aj","ak","al","am",
	"an","ao","ap","aq","ar","as","at","au","av","aw","ax","ay","az",
	"ba","bb","bc","bd","be","bf","bg","bh","bi","bj","bk","bl","bm",
	"bn","bo","bp","bq","br","bs","bt","bu","bv","bw","bx","by","bz",
	"ca","cb","cc","cd","ce","cf","cg","ch","ci","cj","ck","cl","cm",
	"cn","co","cp","cq","cr","cs","ct","cu","cv","cw","cx","cy","cz",
	"da","db","dc","dd","de","df","dg","dh","di","dj","dk","dl","dm",
	"dn","do","dp","dq","dr","ds","dt","du","dv","dw","dx","dy","dz");

# x/y dimensions of the map

$numxval = 40;
$numyval = 40;	

@data = `ls -1 *.html`;
#"<files.dat";
#open FILE or die "test failed for $!";

#@data = <FILE>;
#close(FILE);


#init parameters

for ($i=0;$i<=100;$i++) {
  for ($j=0;$j<=100;$j++) {
    $map[$i][$j] = 0;
    $type[$i][$j] = "\0";
    $garrison[$i][$j] = 0;
    $gate[$i][$j] = -999;
    $annotation[$i][$j] = "";
    $pop[$i][$j] = "";
  }
}
$cities = 0;

#init map with info

foreach $line (@data) {
  $yy = substr($line,0,2);
  $xx = substr($line,2,2);
  $xnum = 9999;
  $ynum = 9999;
  for($i=0;$i<=100;$i++) {
    if ($xx eq $x[$i]) { $xnum = $i }
    if ($yy eq $y[$i]) { $ynum = $i }
  }
  if ($ynum != 9999 && $xnum != 9999) {$map[$xnum][$ynum] = 1}
}
$mapfile = $ARGV[0];
$xmin = $ARGV[1];
$xmax = $ARGV[2];
$ymin = $ARGV[3];
$ymax = $ARGV[4];
$maptype = $ARGV[5];

#gate info
if ($maptype eq "gate") {
  $FILE = "<gates.dat";
  open FILE or die "open failed for $FILE!";
  @gatedata = <FILE>;
  close(FILE);

  $numgates = 0;
  foreach $line (@gatedata) {
    $yy = substr($line,0,2);
    $xx = substr($line,2,2);
    $xnum[$numgates] = 9999;
    $ynum[$numgates] = 9999;
    for($i=0;$i<=100;$i++) {
      if ($xx eq $x[$i]) { $xnum[$numgates] = $i }
      if ($yy eq $y[$i]) { $ynum[$numgates] = $i }
    }
    if ($ynum[$numgates] != 9999 && $xnum[$numgates] != 9999) {
      $gate[$xnum][$ynum] = substr($line,5,2);
      $gatedist[$numgates] = substr($line,5,2);
    }
    $numgates++;
  }
  for ($i = 0;$i<$numgates;$i++) {
    for ($j = 0;$j<100;$j++) {
      for ($k = 0;$k<100;$k++) {
        $xdist = abs($j-$xnum[$i]);
        if ($xdist > 50) {$xdist = 100 - $xdist};
        $ydist = abs($k-$ynum[$i]);
        if ($ydist > 50) {$ydist = 100 - $ydist};
        if ($gatedist[$i] > ($xdist+$ydist)) {$gate[$j][$k] = -1};
      }
    }
    $line2[$xnum[$i]][$ynum[$i]] = "<b>" . $gatedist[$i] . "</b>";
    if ($gatedist[$i] == 0) {$line2[$xnum[$i]][$ynum[$i]] = "<b>GATE</b>";}
  }            
}

#
#get annotation info
#

$annoyes = 0;
$FILE = "<anno.dat";
if (-e "anno.dat") {
  open FILE or die "test failed for $!";

  @anno = <FILE>;
  close(FILE);
  $annoyes = 1;
  foreach $line (@anno) {
    ($annoprov,$annotext) = split(':',$line);
    print "anno=",$annoprov," ",$annotext;
    $yy = substr($annoprov,0,2);
    $xx = substr($annoprov,2,2);
    $xnum = 9999;
    $ynum = 9999;
    for($i=0;$i<=100;$i++) {
      if ($xx eq $x[$i]) { $xnum = $i }
      if ($yy eq $y[$i]) { $ynum = $i }
    }
    if ($ynum != 9999 && $xnum != 9999) {$annotation[$xnum][$ynum] = $annotext}
  }
} else {
  print "No anno.dat\n";
}

#
#find out which provinces are garrisoned to our castles
#

if ($maptype eq "full" && -e "garrison.html") {
  $FILE = "<./garrison.html";
  open FILE or die "test failed for $FILE";
  @data = <FILE>;
  close(FILE);

  $count=0;
  foreach $line (@data) {
    $_ = $line;
    if (/-----------------------------------/) {
      if (substr($data[$count-1],15) =~ /\[([^\]]*)\]/) {
        $yy = substr($1,0,2);
        $xx = substr($1,2,2);
        $xnum = 9999;
        $ynum = 9999;
        for($i=0;$i<=100;$i++) {
          if ($xx eq $x[$i]) { $xnum = $i }
          if ($yy eq $y[$i]) { $ynum = $i }
        }
        if ($ynum != 9999 && $xnum != 9999) {$garrison[$xnum][$ynum] = 1}
      }
    } 
    $count++;
  }
}
#find other info....

for ($j1=$ymin;$j1<=$ymax;$j1++) { 
  for ($i1=$xmin;$i1<=$xmax;$i1++) {
    $i = $i1;
    if ($i1 < 0) { $i = $numxval + $i1 };
    $j = $j1;
    if ($j1 < 0) { $j = $numyval + $j1 };

    if ($map[$i][$j] !=0) {$line1[$i][$j] = "";}
    if ($garrison[$i][$j]==1) {$line1[$i][$j] = $line1[$i][$j] . "<b>G</b>";}

    if ($map[$i][$j] > 0) {
      $FILE = "<./" . $y[$j] . $x[$i] . ".html";
      open FILE or die "test failed for $!";
      @data = <FILE>;
      close(FILE);

      $count = 0;
      $routes = 0;
      $firstline = 0;
      $inner = 0;
#      $garrison = 0;
      foreach $line (@data) {
        $_ = $line;
	if (/------------------------------------------------------------------------/) {
	  @fields = split /,/,$data[$count-1];
	  if ($firstline == 0) { $type[$i][$j] = $fields[1] }
	  $firstline = 1;
#print $y[$j],$x[$i],"=",$type[$i][$j],"--\n";
          if ($data[$count-1] =~ /peasants: /) { $pop[$i][$j] = 1; }
	}
	if (/Routes leaving/) { $routes = 1};
	if ($routes > 0) {
	  @fields = split /,/,$data[$count];
	  if (/North/ && ($type[$i][$j-1] eq "\0" || $fields[1] eq " city")) {
	    @fields = split /,/,$data[$count];
	    if ($fields[1] ne " city") {$type[$i][$j-1] =  $fields[1];}
	    if ($fields[1] eq " city") {
	      $city = "\0";
	      if ($data[$count] =~ /\[([^\]]*)\]/ && $provcity[$i][$j-1] != 1) {
                $city = $1;
	        $citylist[$cities] = $city;
	        $cities++;
	        $provcity[$i][$j-1] = 1;
#print "added city for province ",$y[$j-1],$x[$i]," in ",$y[$j],$x[$i],"\n";
	        $line1[$i][$j-1] = $line1[$i][$j-1] . "<B>X</B>";
	      }
	    }
	  }
	  if (/South/ && ($type[$i][$j+1] eq "\0" || $fields[1] eq " city")) {
	    @fields = split /,/,$data[$count];
	    if ($fields[1] ne " city") {$type[$i][$j+1] =  $fields[1];}
	    if ($fields[1] eq " city") {
	      $city = "\0";
	      if ($data[$count] =~ /\[([^\]]*)\]/ && $provcity[$i][$j+1] != 1) {
                $city = $1;
	        $citylist[$cities] = $city;
	        $cities++;
	        $provcity[$i][$j+1] = 1;
#print "added city for province ",$y[$j+1],$x[$i]," in ",$y[$j],$x[$i],"\n";
	        $line1[$i][$j+1] = $line1[$i][$j+1] . "<B>X</B>";
	      }
	    }
	  }
	  if (/West/ && ($type[$i-1][$j] eq "\0" || $fields[1] eq " city")) {
	    @fields = split /,/,$data[$count];
	    if ($fields[1] ne " city") {$type[$i-1][$j] =  $fields[1];}
	    if ($fields[1] eq " city") {
	      $city = "\0";
	      if ($data[$count] =~ /\[([^\]]*)\]/ && $provcity[$i-1][$j] != 1) {
                $city = $1;
	        $citylist[$cities] = $city;
	        $cities++;
	        $provcity[$i-1][$j] = 1;
#print "added city for province ",$y[$j],$x[$i-1]," in ",$y[$j],$x[$i],"\n";
	        $line1[$i-1][$j] = $line1[$i-1][$j] . "<B>X</B>";
	      }
	    }
	  }
	  if (/East/ && ($type[$i+1][$j] eq "\0" || $fields[1] eq " city")) {
	    @fields = split /,/,$data[$count];
	    if ($fields[1] ne " city") {$type[$i+1][$j] =  $fields[1];}
	    if ($fields[1] eq " city") {
	      $city = "\0";
	      if ($data[$count] =~ /\[([^\]]*)\]/ && $provcity[$i+1][$j] != 1) {
                $city = $1;
	        $citylist[$cities] = $city;
	        $cities++;
	        $provcity[$i+1][$j] = 1;
#print "added city for province ",$y[$j],$x[$i+1]," in ",$y[$j],$x[$i],"\n";
	        $line1[$i+1][$j] = $line1[$i+1][$j] . "<B>X</B>";
	      }
	    }
	  }
#
# check for garrisons in "routes leaving" section
#
          if (/Garrison/) {
            if ($data[$count-1] =~ /North/ && $garrison[$i][$j-1] ==0) {
	      $garrison[$i][$j-1] = 2;
	      $line1[$i][$j-1] = $line1[$i][$j-1] . "<font color=red>G</font>";
            }
            if ($data[$count-1] =~ /South/ && $garrison[$i][$j+1] ==0) {
	      $garrison[$i][$j+1] = 2;
	      $line1[$i][$j+1] = $line1[$i][$j+1] . "<font color=red>G</font>";
            }
            if ($data[$count-1] =~ /West/ && $garrison[$i-1][$j] ==0) {
	      $garrison[$i-1][$j] = 2;
	      $line1[$i-1][$j] = $line1[$i-1][$j] . "<font color=red>G</font>";
            }
            if ($data[$count-1] =~ /East/ && $garrison[$i+1][$j] ==0) {
	      $garrison[$i+1][$j] = 2;
	      $line1[$i+1][$j] = $line1[$i+1][$j] . "<font color=red>G</font>";
            }
	  }
#
# check for mine shafts
#
	  if (/Down/ && /mine-shaft/) {
            $line1[$i][$j] = $line1[$i][$j] . "M";
	  }

	}	#end of "Routes leaving"
#
# check for garrisons besides ours in this province
#
        if (/Province controlled by/) {
          if ($garrison[$i][$j] == 0) { ($garrison[$i][$j] = 2); }
          if ($garrison[$i][$j] == 2) {
            $line1[$i][$j] = $line1[$i][$j] . "<font color=red>G</font>";
	  }
        }
#
# check inner locations
#

	if (/Inner location/) { 
	  $inner = 1;
	  $routes = 0;
	}
	if ($inner > 0) {
	  @fields = split /,/,$data[$count];
	  if ($fields[1] eq " port city" || $fields[1] eq " city") {
	    $city = "\0";
	    if ($fields[0] =~ /\[([^\]]*)\]/) {
              $city = $1;
	      $citylist[$cities] = $city;
	      $cities++;
	      $provcity[$i][$j] = 1;
#print "added city for province ",$y[$j],$x[$i],"\n";
	    }
	    if ($maptype eq "full") {
	      $line1[$i][$j] = $line1[$i][$j] .  
		"<B><a href=\"".$city.".html\">X</a></B>";
	    }
	    if ($maptype eq "nohtml" || $maptype eq "gate") {
	      $line1[$i][$j] = $line1[$i][$j] . "<B>X</B>";
	    }
	  }
	  if ($fields[1] eq " yew grove") { 
	    $line1[$i][$j] = $line1[$i][$j] . "y"
	  }
	  if ($fields[1] eq " rocky hill") { 
	    $line1[$i][$j] = $line1[$i][$j] . "r"
	  }
	  if ($fields[1] eq " graveyard") { 
	    $line1[$i][$j] = $line1[$i][$j] . "g"
	  }
	  if ($fields[1] eq " sacred grove") { 
	    $line1[$i][$j] = $line1[$i][$j] . "S"
	  }
	  if ($fields[1] eq " oasis") { 
	    $line1[$i][$j] = $line1[$i][$j] . "o"
	  }
	  if ($fields[1] eq " orc-stronghold") { 
	    $line1[$i][$j] = $line1[$i][$j] . "O"
	  }
	  if ($fields[1] eq " pits") { 
	    $line1[$i][$j] = $line1[$i][$j] . "z"
	  }
	  if ($fields[1] eq " pasture") { 
	    $line1[$i][$j] = $line1[$i][$j] . "P"
	  }
	  if ($fields[1] eq " sand pit") { 
	    $line1[$i][$j] = $line1[$i][$j] . "s"
	  }
	  if ($fields[1] eq " mallorn grove") { 
	    $line1[$i][$j] = $line1[$i][$j] . "m"
	  }
	  if ($fields[1] eq " island") { 
	    $line1[$i][$j] = $line1[$i][$j] . "i"
	  }
	  if ($fields[1] eq " inn" || $fields[1] eq " inn\n") { 
	    $line1[$i][$j] = $line1[$i][$j] . "I"
	  }
	  if ($fields[1] eq " temple\n" || $fields[1] eq " temple") { 
	    $line1[$i][$j] = $line1[$i][$j] . "T"
	  }
	  if ($fields[1] eq " tower") { 
	    $line1[$i][$j] = $line1[$i][$j] . "t"
	  }
	  if ($fields[1] eq " castle") { 
	    $line1[$i][$j] = $line1[$i][$j] . "C"
	  }
	  if ($fields[1] eq " cave") { 
	    $line1[$i][$j] = $line1[$i][$j] . "v"
	  }
	  if ($fields[1] eq " poppy field") { 
	    $line1[$i][$j] = $line1[$i][$j] . "p"
	  }
	  if ($fields[1] eq " bog") { 
	    $line1[$i][$j] = $line1[$i][$j] . "b"
	  }
	  if ($fields[1] eq " circle of trees") { 
	    $line1[$i][$j] = $line1[$i][$j] . "c"
	  }
	  if ($fields[1] eq " ring of stones") { 
	    $line1[$i][$j] = $line1[$i][$j] . "n"
	  }
	  if ($fields[1] eq " ruins") { 
	    $line1[$i][$j] = $line1[$i][$j] . "R"
	  }
	  if ($fields[1] eq " lair") { 
	    $line1[$i][$j] = $line1[$i][$j] . "L"
	  }
	  if ($fields[1] eq " faery hill") { 
	    $line1[$i][$j] = $line1[$i][$j] . "f"
	  }
	  if ($fields[1] eq " field" || $fields[1] eq " battlefield") { 
	    $line1[$i][$j] = $line1[$i][$j] . "F"
	  }
	  if ($fields[1] eq " enchanted forest") { 
	    $line1[$i][$j] = $line1[$i][$j] . "e"
	  }
#	  if ($fields[1] eq " mine-shaft") { 
#	    $line1[$i][$j] = $line1[$i][$j] . "M"
#	  }
	}
	$count++;
      }
    }
# do the gate map, deleting other info...
   if ($maptype eq "gate") {
     $line1[$i][$j] = $line2[$i][$j];
   }
   
  }
}

for ($i=0;$i<$cities;$i++) {
  $FILE = "<./" . $citylist[$i] . ".html";
  $testfile = open (FILE);
  if (!testfile) {
    $fname1 = $citylist[$i] . ".html";
    print "meh"; 
    `touch $fname1`;
    next;
  }
#die "test failed for $FILE!";
  @data = <FILE>;
  close(FILE);

  $rumor = 0;
  foreach $line (@data) {
    $_ = $line;
 
    if (/Cities rumored/) { $rumor = 1 }
    if (/Market report/) { $rumor = 0 }

    if ($rumor == 1) {
      $cityid = substr($line,-6,4);
      if ($line ne "\n" && $cityid ne "arby") {
        $yy = substr($cityid,0,2);
        $xx = substr($cityid,2,2);
        $xnum = 9999;
        $ynum = 9999;
        for($j=0;$j<=100;$j++) {
          if ($xx eq $x[$j]) { $xnum = $j }
          if ($yy eq $y[$j]) { $ynum = $j }
        }
        if ($provcity[$xnum][$ynum] < 1) {
	  $line1[$xnum][$ynum] = "<B>X</B>".$line1[$xnum][$ynum];
	  $provcity[$xnum][$ynum] = 1;
#print "added city for province ",$y[$ynum],$x[$xnum],"\n";
	}
#print "rumor = ",$cityid,"---",$citylist[$i],"loc=",$y[$ynum],$x[$xnum],"\n";
      }
    }
  }
}

#print stuff out...
$MAP = ">" . $mapfile;
open MAP or die "test failed for $MAP!";


print MAP <<ENDOUT1;
<html>
<head>
<title>Map</title>
</head>
<body>
<hr>
ENDOUT1
;
if ($maptype eq "full" || $maptype eq "nohtml" || $maptype eq "gate") {
  print MAP "<table border>\n";
}
if ($maptype eq "small") {
  print MAP "<table cellspacing=0 cellpadding=0 border=0>\n";
}

for ($j1=$ymin;$j1<=$ymax;$j1++) {
  print MAP "<tr>\n";
  for ($i1=$xmin;$i1<=$xmax;$i1++) {
    $i = $i1;
    if ($i1 < 0) { $i = $numxval + $i1 };
    $j = $j1;
    if ($j1 < 0) { $j = $numyval + $j1 };

    if ($pop[$i][$j] == 1) {
      $popB = "<b>";
      $popB2 = "</b>";
    } else {
      $popB = "";
      $popB2 = "";
    }

    if ($maptype eq "full" || $maptype eq "nohtml" || $maptype eq "gate") {
      print MAP "<td ";
      $_ = $type[$i][$j];
      if(/desert/i) { print MAP "bgcolor=\"yellow\"" }
      if(/plain/i) { print MAP "bgcolor=\"lime\"" }
      if(/mountain/i) { print MAP "bgcolor=\"brown\"" }
      if(/forest/i) { print MAP "bgcolor=\"green\"" }
      if(/ocean/i) { print MAP "bgcolor=\"aqua\"" }
      if(/swamp/i) { print MAP "bgcolor=\"gray\"" }
      print MAP ">";

#two lines (one line) of info here....
      print MAP $line1[$i][$j],"<br>";
      if ($map[$i][$j] > 0 && $maptype eq "full") {
        print MAP "$popB<a href=\"",$y[$j],$x[$i],".html\">",
		$y[$j],$x[$i],"</a>",$popB2;
      } elsif ($maptype eq "gate") {
        if ($gate[$i][$j] >= -1) {
          print MAP "<font color=\"red\">",$y[$j],$x[$i],"</font>";
        } else {
          print MAP $y[$j],$x[$i];
        }
      } else {
        print MAP $y[$j],$x[$i];
      }
      if ($annoyes) { print MAP "<br><b>$annotation[$i][$j]</b>"; }
      print MAP "</td>\n";
    } 
    if ($maptype eq "small") {
      print MAP "<td width=5 height=5> ";
      $_ = $type[$i][$j];
      $used = 0;
      if(/desert/i && $provcity[$i][$j] != 1) { 
	print MAP "<img src=\"image/yellow5.jpg\" alt=\"",$y[$j],$x[$i],"\">";
	$used = 1; }
      if(/desert/i && $provcity[$i][$j] == 1) { 
	print MAP "<img src=\"image/yellow5-city.jpg\" alt=\"",$y[$j],$x[$i],"\">";
	$used = 1; }
      if(/plain/i && $provcity[$i][$j] != 1) { 
	print MAP "<img src=\"image/lime5.jpg\" alt=\"",$y[$j],$x[$i],"\">";
	$used = 1; }
      if(/plain/i && $provcity[$i][$j] == 1) { 
	print MAP "<img src=\"image/lime5-city.jpg\" alt=\"",$y[$j],$x[$i],"\">";
	$used = 1; }
      if(/mountain/i && $provcity[$i][$j] != 1) { 
	print MAP "<img src=\"image/brown5.jpg\" alt=\"",$y[$j],$x[$i],"\">";
	$used = 1; }
      if(/mountain/i && $provcity[$i][$j] == 1) { 
	print MAP "<img src=\"image/brown5-city.jpg\" alt=\"",$y[$j],$x[$i],"\">";
	$used = 1; }
      if(/forest/i && $provcity[$i][$j] != 1) { 
	print MAP "<img src=\"image/green5.jpg\" alt=\"",$y[$j],$x[$i],"\">";
	$used = 1; }
      if(/forest/i && $provcity[$i][$j] == 1) { 
	print MAP "<img src=\"image/green5-city.jpg\" alt=\"",$y[$j],$x[$i],"\">";
	$used = 1; }
      if(/ocean/i && $provcity[$i][$j] != 1) { 
	print MAP "<img src=\"image/aqua5.jpg\" alt=\"",$y[$j],$x[$i],"\">";
	$used = 1; }
      if(/ocean/i && $provcity[$i][$j] == 1) { 
	print MAP "<img src=\"image/aqua5-city.jpg\" alt=\"",$y[$j],$x[$i],"\">";
	$used = 1; }
      if(/to Ocean/i && $provcity[$i][$j] != 1) { 
	print MAP "<img src=\"image/aqua5.jpg\" alt=\"",$y[$j],$x[$i],"\">";
	$used = 1; }
      if(/to Ocean/i && $provcity[$i][$j] == 1) { 
	print MAP "<img src=\"image/aqua5-city.jpg\" alt=\"",$y[$j],$x[$i],"\">";
	$used = 1; }
      if(/swamp/i && $provcity[$i][$j] != 1) { 
	print MAP "<img src=\"image/grey5.jpg\" alt=\"",$y[$j],$x[$i],"\">";
	$used = 1; }
      if(/swamp/i && $provcity[$i][$j] == 1) { 
	print MAP "<img src=\"image/grey5-city.jpg\" alt=\"",$y[$j],$x[$i],"\">";
	$used = 1; }
      if ($used == 0 && $provcity[$i][$j] == 1) {
	print MAP "<img src=\"image/none5-city.jpg\" alt=\"",$y[$j],$x[$i],"\">"}
      print MAP "</td>\n";
    }
  }
  print MAP "</tr>\n";
}
print MAP "</table>\n";

print MAP <<ENDSTUFF1;
<hr>
<table>
<tr>
<td bgcolor="yellow">desert<br></td>
<td bgcolor="lime">plain</td>
<td bgcolor="brown">mountain</td>
<td bgcolor="green">forest</td>
<td bgcolor="aqua">ocean</td>
<td bgcolor="gray">swamp</td>
</tr>
</table>
ENDSTUFF1
;

#only print this list for the big map

if ($maptype eq "full" || $maptype eq "nohtml") {
print MAP <<ENDSTUFF2;
<b>
X=city;
b=bog;
C=castle;
c=circle of trees;
e=enchanted forest;
f=faery hill;
G=garrison;
<br>
g=graveyard;
I=inn;
i=island;
L=lair;
M=mine shaft;
m=mallorn grove;
n=ring of stones;
<br>
O=orc-stronghold;
o=oasis;
P=pasture
p=poppy field;
r=rocky hill;
R=ruins;
S=sacred grove;
<br>
s=sand pit;
T=temple;
t=tower;
v=cave;
y=yew grove; 
z=pits
</b>
ENDSTUFF2
;
}
if ($maptype eq "gate") {
print MAP <<ENDSTUFF3; 
<br>Bold numbers are distance to the nearest gate.  Red-lettered provinces
have been mathematically eliminated as possibly holding a gate.
ENDSTUFF3
;
}
    
print MAP "</body></html>\n";
close(MAP);

#print some interesting info:

$numprov = 0;
for ($i=0;$i<100;$i++) {
  for ($j=0;$j<100;$j++) {
    if ($map[$i][$j]) { $numprov++; };
  }
}

print "Provinces visited = ",$numprov,"\n";
print "Number of cities visited: ",$cities,"\n";
exit;
