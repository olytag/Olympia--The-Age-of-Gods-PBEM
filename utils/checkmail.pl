#!/usr/bin/perl

# check the incoming email directory for new files, and copy them to the 
# lib/spool/m file for parsing by the continuously-running "olytag -e"
# orders eater

$mailDir = "/home/tagtest/incoming/new";
$oldMailDir = "/home/tagtest/incoming/old";
if (!-e $oldMailDir) { `mkdir -p $oldMailDir`; }
$spoolFile = "/home/tagtest/lib/spool/m";
$charEmailFile = "/home/tagtest/lib/char_email.txt";
$moderator_email = "moderator\@olytag.com";
$game_email = "tagtest\@olytag.com";	#orders email address

$webpass = "missiveZZZ";	#set by the web page - change it and make them match
$gameno = 1;

while (1>0) {	#run forever

  if (!-e $mailDir) { print "no mail dir!\n"; exit; }
  chdir ($mailDir);
  @files = `ls -1`;

  # check all new files in the incoming mail directory

  foreach $file (@files) {
    chomp $file;
    chomp $file;
    if (-e $spoolFile) {

      # don't overwrite the spool file -- wait for it to clear

      for ($i=0;$i<60;$i++) {	# then ends up waiting 10 minutes
        sleep 10;		#check every 10 seconds
        if (! -e $spoolFile) {
	  last;
        }
	#if ($i == 9) {
	#  # too much delay -- exit and send error message
	#  print "mail spool delayed too much -- exiting\n";
	#  exit;
        #}
      }
      parse_and_save();

    } else {
      parse_and_save();
    }
    sleep 10;		# pause enough for each to be processed
  }
  sleep 60;
}

sub parse_and_save()
{
  #print "Working on $file\n";

  # read in entire file

  open FILE,"<$file";
  @data = <FILE>;
  close FILE;

  # email header information

  @from = grep (/^From/,@data);
  @subject = grep (/^Subject/,@data);
  @replyto = grep (/^Reply-to/,@data);
  $from[0] =~ /<(.*)>/;
  @therest = grep (!/^From/,@data);

  # we are going to write out a simple file with just the orders included
  # in it, since most email clients send along a lot of junk (HTML, etc)
  # that isn't needed for out simple order queueing purpose

  open FILE,">/tmp/$file.tmp";
  #print FILE substr($from[0],0,4) . substr($from[0],5);
  $from_address = $1;
  print FILE "From $1\n";
  print FILE $subject[0];
  print FILE $replyto[0];
  print FILE "\n";

  #initialize forwarding info (if captured):

  $forward = 0;		#if 1, we are inside the forward text block
  $forwardmessage = "";
  $forward_id = "";
  $from_id = "";
  $forwardflag = 0;
  $printme = 0;

  # write everything after the begin line

  foreach $line (@data) {
    #ignore HTML
    if ($line =~ /Content-Type: text\/html/) { last; }

    # handling for the #forwardto command (usually ignored)

    if ($forward == 1) {
      if ($line =~ /^#fromid:/) {
        ($junk,$from_id) = split /:/,$line;
        chomp $from_id;
      }
      if ($line =~ /^#password:/) {
        ($junk,$mypwd) = split /:/,$line;
        chomp $mypwd;
      }
      if ($line =~ /^#end/) {
        $forward = 0;
      } else {
        if ($line !~ /^#/) {
          $forwardmessage .= $line;
        }
      }
    }

    # if it contains a "#forwardto:" command, then we abort the orders
    # and try to forward the message instead (set a flag, here)

    if ($line =~ /^#forwardto:/) {
      ($junk,$forward_id) = split /:/,$line;
      chomp $forward_id;
      $forward = 1;
      $forwardflag = 1;
    }

    # find the start of the orders - this should drop all the stuff that
    # isn't plain text

    if ($line =~ /begin/i) { $printme = 1; }

    # strip some junk from yahoo, gmail, email clients, etc. from the line

    $line =~ s/=A0/ /g;
    $line =~ s/=20/ /g;
    $line =~ s/=93/\"/g;
    $line =~ s/=94/\"/g;
    $line =~ s/=E2=80=9C/\"/g;
    $line =~ s/=E2=80=9D/\"/g;
    $line =~ s/=E2=80=99/\'/g;
    if ($printme && $line !~ /^#/) { print FILE $line; }
  }

  #if this is a message to be forwarded, then do so, and ignore orders

  if ($forwardflag == 1) {
    forwardto(); 
    close FILE;
    `mv $file $oldMailDir`;
    `rm /tmp/$file.tmp`;	#delete this so it doesn't get spooled next time
    return;		#we are done here, so don't do anything to the spool

  # otherwise, move the incoming message to the archive and copy the orders
  # to the TAG spool file

  } else {
    close FILE;
    #print `cat /tmp/$file.tmp`;
    `mv $file $oldMailDir`;
    `mv /tmp/$file.tmp $spoolFile`;
  }
# print $file,"\n";
# print $oldMailDir,"\n";
# print $mailDir,"\n";
# print $spoolFile,"\n";
  return;
}
sub forwardto()
{ 
  if (length($from_address) < 5 || ($from_address !~ /\@/ && $from_address !~ /website/)) {
    print "Not a valid email address\n";
    return;
  }

  # first check to see if this came from a valid email address

  $valid_pwd = 0;
  $forward_email = "";
  $from_char = "";
  $forward_char = "";

  open CHARFILE,"<$charEmailFile" or die "failed to forward email\n"; 
  @charlist = <CHARFILE>;
  foreach $line (@charlist) {
    ($id,$junk,$junk,$email,$charname,$pwd) = split /\t/,$line;
    chomp $pwd;
    # match both fromid and password

    if (($mypwd eq $pwd || $mypwd eq $webpass)  && $id =~ /$from_id/) { 
      $valid_pwd = 1;
      $from_char = $charname;
      #if it came from the web site, then set the from_address so
      #we can send an Ack email
      if ($from_address =~ /website/) { $from_address = $email; }
    }

    # or match the from address and password

    if ($from_address =~ /$email/ && $from_id eq "" && $mypwd eq $pwd) { 
      #this should grab the first instance of an email address, which
      #in theory should be the faction ID 
      $valid_pwd = 1;
      $from_id = $id;
      $from_char = $charname;
    } 
    if ($forward_id =~ /$id/) {
      $forward_email = $email;
      $forward_char = $charname;
    }
  }

  #if we didn't find a matching password for either the email address or
  #the fromid

  if (!$valid_pwd) {
    open MSGFILE,">/tmp/$file.tmpmsg_fail";
    print MSGFILE "From: $game_email\n";
    print MSGFILE "To: $from_address\n";
    print MSGFILE "Bcc: $moderator_email\n";
    print MSGFILE "Subject: TAG Diplomatic message - failed\n";
    print MSGFILE "\n\n";
    print MSGFILE "$from_address or password is not associated with ID $from_id.\n\n";
    print MSGFILE "Diplomatic missive not delivered.  Please use one of\n";
    print MSGFILE "your valid noble IDs or your faction ID and send the\n";
    print MSGFILE "message with the password associated with your accout.\n";
    print MSGFILE "Or visit: http://olytag.com/$gameno and log in to your account.\n\n";
    print MSGFILE "Also, HTML \"Rich Text\" emails are not allowed - please be\n";
    print MSFGILE "sure to use plain test in your email program.\n";
    print MSGFILE "------------- Original Message ------------\n\n";
    print MSGFILE @data;
    close MSGFILE;
    `msmtp -t < /tmp/$file.tmpmsg_fail`;
    `rm /tmp/$file.tmpmsg_fail`;
  #} elsif ($forward_email eq "") {
  #  open MSGFILE,">/tmp/$file.tmpmsg";
  #  print MSGFILE "From: $game_email\n";
  #  print MSGFILE "To: $from_address\n";
  #  print MSGFILE "Bcc: $moderator_email\n";
  #  print MSGFILE "Subject: TAG Diplomatic message - failed\n";
  #  print MSGFILE "\n\n";
  #  print MSGFILE "[$forward_id] may not be valid\n\n";
  #  print MSGFILE "\n----------------------------\n";
  #  close MSGFILE;
  #  `msmtp -t < /tmp/$file.tmpmsg`;
  #  `rm /tmp/$file.tmpmsg`;

  #otherwise we write out the message and send it, along with an ack
  #to the sender

  } else {
    if ($forward_email ne "") { #only send if the recipient exists
      open MSGFILE,">/tmp/$file.tmpmsg";
      print MSGFILE "From: $game_email\n";
      print MSGFILE "To: $forward_email\n";
      print MSGFILE "Bcc: $moderator_email\n";
      print MSGFILE "Subject: TAG Diplomatic message\n";
      print MSGFILE "\n\n";
      print MSGFILE "Message from $from_char [$from_id] to $forward_char [$forward_id]\n\n";
      print MSGFILE $forwardmessage;
      print MSGFILE "\n----------------------------\n";
      print MSGFILE "\n";
      print MSGFILE "To respond, send a message to $game_email with\n";
      print MSGFILE "#forwardto:$from_id\n";
      print MSGFILE "#fromid:$forward_id\n";
      print MSGFILE "#password:PASSWORD\n";
      print MSGFILE "<your message here>\n";
      print MSGFILE "#end\n";
      print MSGFILE "(no spaces allowed between the colons and IDs)\n\n";
      print MSGFILE "Or visit: http://olytag.com/$gameno and log in to your account.\n\n";
      close MSGFILE;
#print "wrote file, now mailing...\n";
      `msmtp -t < /tmp/$file.tmpmsg`;
      `rm /tmp/$file.tmpmsg`;
    }

    # Now send an Ack to the initiating faction
    open MSGFILE,">/tmp/$file.tmpmsg_ack";
    print MSGFILE "From: $game_email\n";
    print MSGFILE "To: $from_address\n";
    print MSGFILE "Bcc: $moderator_email\n";
    print MSGFILE "Subject: TAG Diplomatic message - sent\n";
    print MSGFILE "\n\n";
    print MSGFILE "Your message was forwarded as shown below.  This does not\n";
    print MSGFILE "guarantee receipt of the message, but an attempt was made\n";
    print MSGFILE "on your behalf.  If the noble/faction ID does not exist,\n";
    print MSGFILE "then the message most likely was not delivered to anyone.\n";
    print MSGFILE "\n----------------------------\n\n";
    print MSGFILE "Message from $from_char [$from_id] to [$forward_id]\n\n";
    print MSGFILE $forwardmessage;
    print MSGFILE "\n----------------------------\n";
    close MSGFILE;
    `msmtp -t < /tmp/$file.tmpmsg_ack`;
    `rm /tmp/$file.tmpmsg_ack`;
#   
  }
  return;
}
