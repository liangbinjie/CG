CC = cc
CFLAGS = -I/usr/local/Mesa-3.4/include
LDLIBS = -lX11 -lglut -lGLU -lGL -lm -lXext -lXmu
LDFLAGS = -L/usr/local/Mesa-3.4/lib -L/usr/X11R6/lib

OBJECTS = main.o utils/interface.o config/config.o
OUTPUT = main

$(OUTPUT): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(OUTPUT) $(OBJECTS) $(LDLIBS)


main.o: main.c utils/utils.h utils/interface.h config/config.h
	$(CC) $(CFLAGS) -c main.c -o main.o

interface.o: utils/interface.c utils/interface.h
	$(CC) $(CFLAGS) -c utils/interface.c -o utils/interface.o

config.o: config/config.c config/config.h
	$(CC) $(CFLAGS) -c config/config.c -o config/config.o

clean:
	rm -f *.o
	rm -f utils/*.o
	rm -f $(OUTPUT)
