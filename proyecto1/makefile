CC = cc
CFLAGS = -I/usr/local/Mesa-3.4/include
LDLIBS = -lX11 -lglut -lGLU -lGL -lm -lXext -lXmu
LDFLAGS = -L/usr/local/Mesa-3.4/lib -L/usr/X11R6/lib

OBJECTS = main.o map.o utils/interface.o utils/draw_line.o
OUTPUT = main

$(OUTPUT): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(OUTPUT) $(OBJECTS) $(LDLIBS)


main.o: main.c utils/mesa.h utils/utils.h utils/interface.h utils/coordinates.h
	$(CC) $(CFLAGS) -c main.c -o main.o


map.o: map.c utils/coordinates.h
	$(CC) $(CFLAGS) -c map.c -o map.o


utils/interface.o: utils/interface.c utils/interface.h utils/coordinates.h
	$(CC) $(CFLAGS) -c utils/interface.c -o utils/interface.o

utils/draw_line.o: utils/draw_line.c utils/draw_line.h
	$(CC) $(CFLAGS) -c utils/draw_line.c -o utils/draw_line.o

clean:
	rm -f *.o
	rm -f utils/*.o
	rm -f $(OUTPUT)
