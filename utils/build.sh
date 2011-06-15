export TARGET=/home/tagtest/bin
gcc -o tagacct tagacct.c
gcc -o entab entab.c
gcc -c z.c
gcc -o mailsplit mailsplit.c z.o
gcc -c tags.c
gcc -c conv.c -o conv.o
gcc -o texter texter.c tags.o
gcc -g -o htmler htmler.c tags.o conv.o
gcc -g -o text_filter text_filter.c
gcc -g -o untangle untangle.c
mkdir $TARGET
cp tagacct rep entab scanner tagstatus mailsplit texter htmler untangle text_filter addplayer.pl checkmail.pl map.pl $TARGET
chmod 755 \
	$TARGET/checkmail.pl \
	$TARGET/rep \
	$TARGET/addplayer.pl \
	$TARGET/map.pl
