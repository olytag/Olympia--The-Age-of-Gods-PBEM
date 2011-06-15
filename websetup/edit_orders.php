<html><head><title>TAG Orders Template Setup</title>
<link href="../css/style.css" rel="stylesheet" type="text/css" />
</head>
<body>

<div id="page">
<div id="wrap">
<div id="head">
<div id="title">Olympia: The Age of Gods</div>
<div id="desc">TAG Orders Template Setup</div>
</div><!-- end header -->
<div id="content">
<div class="post" id="post-1">
<h4>TAG Orders Template Setup</h4>

The first section contains anything that you want to see inserted under your
"unit FactionID" persistently each turn.  The main use of this is for "accept"
commands, but anything that the faction unit needs to do every turn could be in there.
For instance, in a two-nation game, you could do this:
<pre>
   accept clear 
   accept Imperial 0	#accept anything for any Imperial noble
   defend		#clear defend list
   defend Imperial	#defend all Imperial nobles
   press 0
   We claim the entire world for ourselves.  Please leave immediately.
   end
</pre>
Or you could list all your persistent accepts:
<pre>
   accept clear 
   accept ad2 0         #Khellendros
   accept fg8 0         #Knights of the Sword
   defend		#clear defense list
   defend ad2		#add units/factions to defense list
   defend fg8
</pre>
<B>60 characters wide max, please</B><br>
<form action="edit_orders_submit.php" method="post"> 
<TEXTAREA NAME="topnotes" ROWS=20 COLS=60>
<?php
$myFile = "orders_topnotes";
$fh = fopen($myFile, 'r');
$theData = fread($fh, filesize($myFile));
fclose($fh);
echo "$theData";
?>
</TEXTAREA>
<p>
Next, list the factions or nobles (if not their whole faction) who you wish to
allow admission to your stacks and buildings that your nobles control.  This 
will generate a list of "admit" orders for every one of your nobles that 
admit all the listed factions.  Like:
<pre>
   ad2 fg8 ke9
</pre>
<B>No more than 10 IDs on a line, please.</b>
<br>
<TEXTAREA NAME="admits" ROWS=3 COLS=50>
<?php
$myFile = "orders_admit";
$fh = fopen($myFile, 'r');
$theData = fread($fh, filesize($myFile));
fclose($fh);
echo "$theData";
?>
</TEXTAREA>
<p>
<input type="submit" />
</form>
<p>
When you are finished, hit the submit button.  Changes to your orders template 
will take place when the next turn runs.



<p>
Or skip this and return to <a href="index.php">Your Faction Page<a>.<p>
</div><!-- end post -->
</div><!-- end content -->
<?php readfile("../../css/menu.inc"); ?>
<div id="foot"></div>
</div><!-- end wrap -->
</div> <!-- end page-->
</body></html>

