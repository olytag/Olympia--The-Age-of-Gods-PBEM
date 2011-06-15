#!/usr/bin/perl

# for use with msmtp

@data=`grep " em " ~/lib/fact/*`;
print @data;
print "To: moderator\@WEBSITE.com\n";
print "Reply-to: moderator\@WEBSITE.com\n";
print "Subject: TAG\n";
print "Bcc: moderator\@WEBSITE.com";
foreach $line (@data) {
  chomp $line;
  ($junk,$junk,$email) = split / /,$line;
  print "," . $email;
}

print "\n\n";

