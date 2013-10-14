#
# Przemysław Pastuszka
# nr albumu: 233 186
# 
# Zadanie na pracownię nr 1 z przedmiotu Sieci Komputerowe
# 
# 5/04/2011
#

CC = gcc
CFLAGS = -Wall -W -std=gnu99
OBJECTS = syswrap.o cksum.o request.o trace_results.o trace_host.o pptrace.o

pptrace: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o pptrace

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o

distclean:
	rm -f *.o
	rm -f pptrace
