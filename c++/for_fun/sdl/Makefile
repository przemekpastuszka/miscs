CC = g++
CFLAGS = -Wall -lSDL -lSDL_image -O2
OBJECTS = app_init.o app_event.o app_update.o app_render.o app_execute.o resources.o app_cleanup.o test.o

pptrace: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o test

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o

distclean:
	rm -f *.o
	rm -f test
