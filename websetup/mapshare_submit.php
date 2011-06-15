<html><head><title>Submission Complete</title>
<link href="../css/style.css" rel="stylesheet" type="text/css" />
</head>
<body>
<div id="page">
<div id="wrap">
<div id="head">
<div id="title">Olympia: The Age of Gods</div>
<div id="desc">Map Sharing: Trusted Factions</div>
</div><!-- end header -->
<div id="content">
<div class="post" id="post-1">

<h3>Thank you for your submission</h3>
<b>You have chosen to share your map data with the following factions on an ongoing basis.  To turn sharing off in the future, please revisit the sharing options page and remove the unwanted factions from the list.</b>
<pre>
<?php
echo $_POST['trusted'];
?>
</pre>
<?php
$myFile = "mapshare.trusted";
$fh = fopen($myFile, 'w') or die("can't open mapshare.trusted file");
fwrite($fh, $_POST['trusted']);
fclose($fh);
system('perl mapshare.pl');
?>
<p>
<a href="mapshare.php">Map Sharing Options</a><p>
<a href="index.php">Your Faction Page<a><p>

</div><!-- end post -->
</div><!-- end content -->
<?php readfile("../../css/menu.inc"); ?>
<div id="foot"></div>
</div><!-- end wrap -->
</div> <!-- end page-->
</body></html>
