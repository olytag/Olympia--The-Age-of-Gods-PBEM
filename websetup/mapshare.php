<html><head><title>TAG Map Sharing</title>
<link href="../css/style.css" rel="stylesheet" type="text/css" />
</head><body>
<div id="page">
<div id="wrap">
<div id="head">
<div id="title">Olympia: The Age of Gods</div>
<div id="desc">Map Sharing: Trusted Factions</div>
</div><!-- end header -->
<div id="content">
<div class="post" id="post-1">

<h4>TAG Map Sharing</h4>

Please list the IDs of the factions with whom you want to automatically share ALL map data with each turn (trusted factions):<br>
<form action="mapshare_submit.php" method="post"> 
<TEXTAREA NAME="trusted" ROWS=10 COLS=30>
<?php
$myFile = "mapshare.trusted";
$fh = fopen($myFile, 'r');
$theData = fread($fh, filesize($myFile));
fclose($fh);
echo "$theData";
?>
</TEXTAREA>
<p>
<!--
Next, list the factions with whom you wish to only share the location information
but not details about noble movements or who is seen in the location.
<br>
<TEXTAREA NAME="loconly" ROWS=10 COLS=30>
<?php
$myFile = "mapshare.loconly";
$fh = fopen($myFile, 'r');
$theData = fread($fh, filesize($myFile));
fclose($fh);
echo "$theData";
?>
</TEXTAREA>
-->
<input type="submit" />
</form>
<p>
<a href="index.php">Your Faction Page<a><p>
</div><!-- end post -->
</div><!-- end content -->
<?php readfile("../../css/menu.inc"); ?>
<div id="foot"></div>
</div><!-- end wrap -->
</div> <!-- end page-->
</body></html>


</body>

