#
# Przemysław Pastuszka
# nr albumu: 233 186
# 
# Zadanie na pracownię nr 3 z przedmiotu Sieci Komputerowe
# 05/06/2011
#

CC = gcc
CFLAGS = -Wall -W -std=gnu99
OBJECTS = server_start.o server_response.o syswrap.o utilities.o ppserver.o

pptrace: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o ppserver

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o

distclean:
	rm -f *.o
	rm -f ppserver
