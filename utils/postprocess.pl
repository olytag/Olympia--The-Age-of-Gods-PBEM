#!/usr/bin/perl
use strict;

my $libdir = "/home/tagtest/lib";
my $webdir = "/home/tagtest/www/1";
my $pwdfile = "/home/tagtest/passwords.1";
my $websetupdir = "/home/tagtest/websetup";

my $mapbounds = "0 39 0 39";


if ($#ARGV != 0) {
  print "Usage: postprocess.pl <turn#>\n";
  exit;
}
my $turn = $ARGV[0];
my $turndir = $libdir . "/save/" . $turn;

if (! -e "$turndir") {
  print "Turn #$turn not found\n";
  exit;
}
chdir $turndir;
print `pwd`;

# write out text format reports to web site

my @factionlist;
my @factionnamelist;
my $id_list = "";
my $i;
my $id_list;
my $fnum = 0;

foreach $i (`ls -1 [0-9]*`) {
  chomp $i;
  my $id = $i;
  my $name = ""; 
  my $stuff = `grep "Olympia:TAG game .* turn .* report for" $i`;
  if ($stuff =~ /report for .*>(.*)~\[(.*)\]/) {
    $id = $2;
    $name = $1;
  }
  $factionlist[$fnum] = $id;
  $factionnamelist[$fnum] = $name;
  $fnum++;
  $id_list .= " $id";
  my $reportsdir = $webdir . "/" . $id . "/reports/" . $turn;
  if (! -e $reportsdir) {
    `mkdir -p $reportsdir`;
  }
  print "Working turn report for $name [$id]\n";
  `rep $turndir/$i > $reportsdir/$id.txt`;
  `cp $reportsdir/$id.txt $webdir/$id/report_latest.txt`;
  my $mapdir = $webdir . "/" . $id . "/info";
  my $mapdir_html = $webdir . "/" . $id . "/info_html";
  if (! -e $mapdir) {
    `mkdir -p $mapdir`;
    `chmod 775 $mapdir`;
    `mkdir -p $mapdir_html`;
    `chmod 775 $mapdir_html`;
  }
  `splitreport.pl $reportsdir/$id.txt $mapdir`;
  `splitreport.pl $reportsdir/$id.txt $mapdir_html 1`;
}

# share map information for each faction

for ($i=0;$i<$fnum;$i++) {
  my $factdir = $webdir . "/" . $factionlist[$i];
  chdir $factdir;
print $factdir,"\n";
  `perl mapshare.pl`;
}
 
# create a map for each faction

for ($i=0;$i<$fnum;$i++) {

  my $factdir = $webdir . "/" . $factionlist[$i];
  my $mapdir = $factdir . "/info";
  my $mapdir_html = $factdir . "/info_html";
  if (! -e $mapdir) {
    `mkdir -p $mapdir`;
  }
  chdir $mapdir;
  print `map.pl map.html $mapbounds full`;  
  print `map.pl map_small.html $mapbounds small`;  
  print `cp map.html $mapdir_html`;  
  `chmod 666 map.html $mapdir_html/map.html`;
  # if new faction, also copy the web startup stuff:
  if (! -e "$factdir/orders_topnotes") {
    `cp -r $websetupdir/* $factdir`;
	#need to be world-writable by the web server:
    `chmod 666 $factdir/*orders*`;
  }
}

# make an index page for each faction

for ($i=0;$i<$fnum;$i++) {

  if ($factionlist[$i] =~ /^[0-9][0-9][0-9]/) { next; } 
  my $factdir = $webdir . "/" . $factionlist[$i];
  my $reportsdir_latest = $factdir . "/reports/" . $turn;
  my $reportsdir = $factdir . "/reports";
  my $mapdir = $factdir . "/info";

  chdir $factdir;
  `makeorders $reportsdir_latest/$factionlist[$i].txt > $factdir/orders.txt`;
  `chmod 666 $factdir/orders.txt`;

  chdir ($reportsdir);
  my @turns = `ls -1t`;	#reverse time order
  my $turnlist = "";
  foreach my $t (@turns) {
    chomp $t;
    $turnlist .= "<a href=\"reports/$t/$factionlist[$i].txt\">$t</a> ";
  }
  my $turnplus1 = $turn+1;
  my $indexpage = <<EOF;
<html><head><title>$factionnamelist[$i]</title>
<link href="../../css/style.css" rel="stylesheet" type="text/css" />
</head><body>
<div id="page">
<div id="wrap">
<div id="head">
<div id="title">Olympia: The Age of Gods</div>
<div id="desc">$factionnamelist[$i] \[$factionlist[$i]\]</div>
</div><!-- end header -->
<div id="content">
<div class="post" id="post-1">
<ul>
<li>Map information:</li>
  <ul>
  <li><a href="info_html/map.html">Personal Map</a></li>
  <li><a href="mapshare.php">map sharing preferences</a></li>
  </ul>
<li>Order tools:</li>
  <ul>
  <li><a href="orderssubmissionform.php">orders submission form</a></li>
  <li><a href="orders.txt">Orders template for turn $turnplus1</a></li>
    <ul>
    <li><a href="edit_orders.php">edit orders template preferences</a></li>
    </ul>
  </ul>
<li><a href="missive.php">Send a diplomatic message</a></li>
<li>Turn Reports: $turnlist</li>
</ul>
</div><!-- end post -->
</div><!-- end content -->
<?php readfile("../../css/menu.inc"); ?>
<div id="foot"></div>
</div><!-- end wrap -->
</div> <!-- end page-->

</body>
</html> 
EOF
;
  open FILE,">$factdir/index.php";
  print FILE $indexpage;
  close FILE; 

  # write out the name of our faction for use by web pages

  open FILE,">$factdir/faction.txt";
  print FILE "$factionnamelist[$i] \[$factionlist[$i]\]\n"; 
  close FILE; 

  # fix permissions

  `chmod 666 $factdir/mapshare.trusted`;
  `chmod 644 $factdir/faction.txt`;

}

#write out player list

my $mainpage = <<EOPP; 
<html><head><title>Olympia: The Age of Gods - player pages</title>
<link href="../css/style.css" rel="stylesheet" type="text/css" />
</head><body>
<div id="page">
<div id="wrap">
<div id="head">
<div id="title">Olympia: The Age of Gods</div>
</div><!-- end header -->
<div id="content">
<div class="post" id="post-1">
</head>
<body>
<h3>TAG Player Pages</h3>
<p/>
<ul>
NEWS: <a href=\"times/$turn.txt\">Latest Issue</a> <a href=\"times\">Back Issues</a>
</ul>
<p/>
<h4>Current players</h4>
<ul>
EOPP
;
for ($i=0;$i<$fnum;$i++) {
  if ($factionlist[$i] =~ /^[0-9][0-9][0-9]/) { next; } 
  $mainpage .=
    "<li>$factionnamelist[$i] \[<a href=$factionlist[$i]>$factionlist[$i]</a>\]</li>\n";
}
$mainpage .= "</ul>\n";

my $tmp = <<EOP;
</div><!-- end post -->
</div><!-- end content -->
<?php readfile("../css/menu.inc"); ?>
<div id="foot"></div>
</div><!-- end wrap -->
</div> <!-- end page-->

</body>
</html> 
EOP
;
$mainpage .= $tmp;

open FILE,">$webdir/index.php" or die "failed to write main index file\n";
print FILE $mainpage;
close FILE;
#write out the Times

if (! -e "$webdir/times") { `mkdir -p $webdir/times`; }
open TIMES,"<$libdir/Times" or die "Failed to open Times\n";
my @timesdata = <TIMES>;
close TIMES;
open TIMESOUT,">$webdir/times/$turn.txt" or die "Failed to write Times\n";
foreach my $line (@timesdata) {
  $line =~ s/\@/\(at\)/g;
  if ($line !~ /From:.*moderator/ &&
      $line !~ /Subject: The Gods/) {
    print TIMESOUT $line;
  }
}
close TIMESOUT;

#`cp $libdir/Times $webdir/times/$turn.txt`; this is done above in a better way
`chmod 644 $webdir/times/$turn.txt`;

#write out htaccess file for protected (all players can access) game directory
#Player directories should already have password protection for their faction

my $htacc =
  "AuthUserFile $pwdfile\n" .
  "AuthGroupFile /dev/null\n" .
  "AuthName FactionIDandPassword\n" .
  "AuthType Basic\n" .
  "\n" .
  "require valid-user\n";
#  "<Limit GET>\n" .
#  "require user admin $id_list\n" .
#  "</Limit>\n";
#Uncomment these four lines to write out the htaccess for the game-level
#directory:
#open FILE,">$webdir/.htaccess" or die "failed to open .htaccess for writing\n";
#print FILE $htacc;
#close FILE;
#`chmod 666 $webdir/.htaccess`;

#
