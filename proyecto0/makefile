CC = cc
CFLAGS = -I/usr/local/Mesa-3.4/include
LDLIBS=-lX11 -lglut -lGLU -lGL -lm -lXext -lXmu
LDFLAGS=-L/usr/local/Mesa-3.4/lib -L/usr/X11R6/lib

OBJECTS = main.o
OUTPUT = main

SRC = main.c utils.c

$(OUTPUT): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(OUTPUT) $(OBJECTS) $(LDLIBS)

clean:
	rm -f *.o
	rm -f $(OUTPUT)
