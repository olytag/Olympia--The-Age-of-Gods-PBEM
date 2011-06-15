<html><head><title>Submission Complete</title>
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
<h4>Thank you for your submission</h4>
<p></p>
<b>Top notes:</b>
<pre>
<?php
echo $_POST['topnotes'];
?>
</pre>
<p></p>
<b>Admit List:</b>
<pre>
<?php
echo $_POST['admits'];
?>
</pre>
<?php
$myFile = "orders_topnotes";
$fh = fopen($myFile, 'w') or die("can't open topnotes file");
fwrite($fh, $_POST['topnotes']);
fclose($fh);
$myFile = "orders_admit";
$fh = fopen($myFile, 'w') or die("can't open admit file");
fwrite($fh, $_POST['admits']);
fclose($fh);
echo system('perl makeorders report_latest.txt > orders.txt');
?>
<p></p>
<ul>
<li><a href="edit_orders.php">Orders Template Editing</a></li>
<li><a href="index.php">Your Faction Page<a><p></li>
<li><a href="orders.txt">Your Orders Template<a></li>
</ul>
</div><!-- end post -->
</div><!-- end content -->
<?php readfile("../../css/menu.inc"); ?>
<div id="foot"></div>
</div><!-- end wrap -->
</div> <!-- end page-->
</body></html>

