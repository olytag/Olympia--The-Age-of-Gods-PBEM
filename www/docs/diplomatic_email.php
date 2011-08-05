<html>
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
<div id="desc">Sending Diplomatic Missives</div>
</div><!-- end header -->
<div id="content">
<div class="post" id="post-1">

<h2>Sending Diplomatic Missives</h2>
<p/>
<p/>

<p>A game like Olympia:TAG is usually a lot more fun when you are interacting
with the other players rather than going at it alone.  In many cases, it
is essential for the survival of your faction, too.</p>

<p>The TAG email interface is set up to allow you to send messages to
other nobles (or factions) from a specific noble of yours, or from your 
faction ID.  This masks your email address so the noble's owner is 
concealed.  Essentially, it is a double-blind diplomatic email server - 
the nobles involved can have a discussion without revealing their lord.</p>

<p>To use this feature, send a message to the orders parser that includes:
<blockquote>
<pre>
#forwardto:aa0
#fromid:zz9
#password:PASSWORD
your message here
#end
</pre>
</blockquote>
<p/>
<p/>
There are no spaces allowed between the colons and IDs, and you should
not send a diplomatic email in the same message as your orders (the orders
are ignored if a <i>#forwardto</i> tag is seen in the email.)  "aa0" is the 
receiving faction's (or noble's) ID, "zz9" is the ID (noble or faction) 
you wish to send from, and "PASSWORD" is your faction password (to prevent
spoofing.)</p>
<p>You should get an acknowlegement that looks like this:
<blockquote>
<pre>
Your message was forwarded as shown below.  This does not
guarantee receipt of the message, but an attempt was made
on your behalf.  If the noble/faction ID does not exist,
then the message most likely was not delivered to anyone.

----------------------------

Message from The Swamp People [aa0] to [r8e]

Dear Sir Melon,

Welcome to our fine swamp!  You are welcome to trade in the 
city, but as the peasant population is still very low, here, 
we kindly request that you cease all recruiting activities
for a few months until the population is more robust (at least
1100 peasants).

Thank you for your understanding and cooperation.

Boggy Bob
</pre>
</blockquote>
<p/><p/>
<p> And the receiving noble's owner will get a message that looks like this:</p>

<blockquote>
<pre>
Message from The Swamp People [aa0] to Sir Melon [r8e]

Dear Sir Melon,

Welcome to our fine swamp!  You are welcome to trade in the 
city, but as the peasant population is still very low, here, 
we kindly request that you cease all recruiting activities
for a few months until the population is more robust (at least
1100 peasants).

Thank you for your understanding and cooperation.

Boggy Bob

----------------------------

To respond, send a message to ... with
#forwardto:aa0
#fromid:r8e
#password:PASSWORD
your message here
#end
(no spaces allowed between the colons and IDs)
</pre>
</blockquote>
</div><!-- end post -->
</div><!-- end content -->
<?php readfile("../css/menu.inc"); ?>

<div id="foot"></div>
</div><!-- end wrap -->
</div> <!-- end page-->
</body>
