<html><head><title>TAG Orders Template Setup</title>
<link href="../css/style.css" rel="stylesheet" type="text/css" />
</head>
<body>

<div id="page">
<div id="wrap">
<div id="head">
<div id="title">Olympia: The Age of Gods</div>
<div id="desc">Diplomatic Message Form</div>
</div><!-- end header -->
<div id="content">
<div class="post" id="post-1">
<h4>TAG Diplomatic Message</h4>

You may type your message in the area below.

<B>60 characters wide max, please</B><br>
<form action="missivesubmit.php" method="post"> 
From (your faction or noble ID):<input type="text" name="from" value=
<?php
$current_dir = dirname($_SERVER['PHP_SELF']);
$basedir = basename($current_dir);
echo "\"$basedir\"";
?>
 maxlength=4><p/>
To (receiving faction's or noble's ID):<input type="text" name="to" maxlength=4><p/>
Your message:<p/>
<TEXTAREA NAME="message" ROWS=40 COLS=60>
your message here
</TEXTAREA>
<p>
<input type="submit" />
</form>
<p>
When you are finished, hit the submit button.  Please check your email
for the results of the submission.

<p>
Or skip this and return to <a href="index.php">Your Faction Page<a>.<p>
</div><!-- end post -->
</div><!-- end content -->
<?php readfile("../../css/menu.inc"); ?>
<div id="foot"></div>
</div><!-- end wrap -->
</div> <!-- end page-->
</body></html>

