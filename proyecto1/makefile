CC = cc
CFLAGS = -I/usr/local/Mesa-3.4/include
LDLIBS=-lX11 -lglut -lGLU -lGL -lm -lXext -lXmu
LDFLAGS=-L/usr/local/Mesa-3.4/lib -L/usr/X11R6/lib

OBJECTS = main.o utils.o interface.o
OUTPUT = main

$(OUTPUT): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(OUTPUT) $(OBJECTS) $(LDLIBS)

main.o: main.c utils/mesa.h utils/utils.h interface.h

clean:
	rm -f *.o
	rm -f $(OUTPUT)
