#
# Przemysław Pastuszka
# nr albumu: 233 186
# 
# Zadanie na pracownię nr 2 z przedmiotu Sieci Komputerowe
# 21/04/2011
#

CC = gcc
CFLAGS = -Wall -W -std=gnu99
OBJECTS = syswrap.o recieve.o send.o queue.o main.o

pptrace: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o ppclient

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o

distclean:
	rm -f *.o
	rm -f ppclient
