<html><head><title>TAG Map Sharing</title></head>
<body>

<h4>TAG Map Sharing</h4>

Please list the IDs of the factions with whom you want to automatically share ALL map data with each turn (trusted factions - <b>one ID per line, please</b>):<br>
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
</body></html>

