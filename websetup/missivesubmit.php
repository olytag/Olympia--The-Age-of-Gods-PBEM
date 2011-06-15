<html><head><title>Submission Complete</title>
<link href="../css/style.css" rel="stylesheet" type="text/css" />
</head>
<body>
<div id="page">
<div id="wrap">
<div id="head">
<div id="title">Olympia: The Age of Gods</div>
<div id="desc">TAG Diplomatic Missivep</div>
</div><!-- end header -->
<div id="content">
<div class="post" id="post-1">
<h4>Thank you for your submission</h4>
<ul>
<li><a href="index.php">Your Faction Page<a><p></li>
</ul>
<p></p>
<b>Message Submitted:</b>
<pre>
<?php
$to = $_POST['to'];
$from = $_POST['from'];
echo "From [$from] to [$to]:<p/>\n";
echo $_POST['message'];
?>
<?php
$myFile = "orderssubmit.tmp";
$fh = fopen($myFile, 'w') or die("can't open orders submission file");
$current_dir = dirname($_SERVER['PHP_SELF']);
$basedir = basename($current_dir);
$to = $_POST['to'];
$from = $_POST['from'];
$pass = "missiveM99";
fwrite($fh, "From: <website $current_dir $basedir>\n\n");
fwrite($fh, "#forwardto:$to\n");
fwrite($fh, "#fromid:$from\n");
fwrite($fh, "#password:$pass\n");
fwrite($fh, $_POST['message']);
fwrite($fh, "\n");
fwrite($fh, "#end\n");
fclose($fh);
echo system('./orderssubmit.pl');
?>
<p></p>
</div><!-- end post -->
</div><!-- end content -->
<?php readfile("../../css/menu.inc"); ?>
<div id="foot"></div>
</div><!-- end wrap -->
</div> <!-- end page-->
</body></html>

