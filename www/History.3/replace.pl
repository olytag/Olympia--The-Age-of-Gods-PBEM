#!/usr/bin/perl

foreach $file (`ls -1 turn*htm*`) {
  open FILE,"<$file" or die "oops\n";
  @data = <FILE>;
  close FILE;

  ($first,$last) = split /\./,$file;
  $newfile = $first . ".php";
  print "Working on $newfile\n";

  open FILE,">$newfile" or die "failed new file";
print FILE<<EOD1;
<head>
<title>Olympia: The Age of Gods</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<link href="../css/style.css" rel="stylesheet" type="text/css" />
</head>
<body>
<div id="page">
<div id="wrap">
<div id="head">
<div id="title">Olympia: The Age of Gods</div>
<div id="desc">The Annals of the Empire</div>
</div><!-- end header -->
<div id="content">
<div class="post" id="post-1">
EOD1
;
  $start = 0;
  foreach $line (@data) {
    if ($start == 1) { 
      if ($line =~ /EndEditable/) {
        $start = 0;
      } else {
        print FILE $line;
      }
    } 

    if ($line =~ /BeginEditable..1..-->/) { 
      ($tmp1,$tmp2) = split /BeginEditable..1..-->/,$line;
      print FILE $tmp2;
      print $tmp2;
      $start = 1;
    }
  }
print FILE<<EOD2;

</div><!-- end post -->
</div><!-- end content -->
<?php readfile("../css/menu.inc"); ?> 
<div id="foot"></div>
</div><!-- end wrap -->
</div> <!-- end page-->
</body>
EOD2
;
  close FILE;
}
