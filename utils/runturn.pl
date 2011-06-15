#!/usr/bin/perl

$home = "/home/tagtest";

#archive the previous turn

if (! -e "$home/archive") { `mkdir -p $home/archive` ; } 
chdir ($home);
$file = $home . "/archive/" . time() . ".tgz";
`tar cvzf $file accounting lib www`;

# stop the processing of the input spool / order eater

`touch $home/pause`;
`touch $home/lib/spool/stop`;
sleep 60;
print `tagstatus`;

# run the turn, and email

print `olytag -l $home/lib -r -S`;	#  (add players, run, save)

# write out the character email list

`perl $home/bin/charemail.pl > $home/lib/char_email.txt`;

#EXIT for now - mail and post-process manually
exit;

print `olytag -l $home/lib -M`;	#  (mail) - DOESN'T WORK PROPERLY!

#print `olytag -l $home/lib -a -r -SM`;	#  (add players, run, save, mail)
#print `olytag -l $home/lib -r -S`;	#  (run and save)
#exit;

#restart the turn scanner

`/bin/rm $home/pause`;
system ("scanner &");	# (restart order eater)
print `tagstatus`;

# do some post-processing

#postprocess.pl turnnumber
