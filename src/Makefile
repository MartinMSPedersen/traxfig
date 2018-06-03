# need a ansi C compiler 
CC = gcc 
CFLAGS = -Wall -Werror
 
traxfig : traxfig.o config.o messages.o move.o parse.o print.o psoutput.o htmloutput.o
	echo ... 
	$(CC) $(CFLAGS) -o traxfig traxfig.o config.o messages.o move.o parse.o print.o psoutput.o htmloutput.o
	echo "Have you thought of changing path in traxfig.h ?" 
 
traxfig.o: traxfig.h messages.h move.h parse.h config.h print.h psoutput.h htmloutput.h 
	echo -n ... 
	$(CC) $(CFLAGS) -c traxfig.c 
 
psoutput.o: psoutput.h traxfig.h 
	echo -n ... 
	$(CC) $(CFLAGS) -c psoutput.c 
 
htmloutput.o: htmloutput.h traxfig.h 
	echo -n ... 
	$(CC) $(CFLAGS) -c htmloutput.c 
 
config.o: traxfig.h config.h 
	echo -n ... 
	$(CC) $(CFLAGS) -c config.c 
 
messages.o: messages.h traxfig.h 
	echo -n ... 
	$(CC) $(CFLAGS) -c messages.c 
 
move.o: move.h 
	echo -n ... 
	$(CC) $(CFLAGS) -c move.c
 
parse.o: parse.h 
	echo -n ...
	$(CC) $(CFLAGS) -c parse.c
 
print.o: traxfig.h print.h move.h
	echo -n ...
	$(CC) $(CFLAGS)  -c print.c
 
clean:
	rm -f traxfig core *.o
 
