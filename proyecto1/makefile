CC = cc
CFLAGS = -I/usr/local/Mesa-3.4/include
LDLIBS = -lX11 -lglut -lGLU -lGL -lm -lXext -lXmu
LDFLAGS = -L/usr/local/Mesa-3.4/lib -L/usr/X11R6/lib

OBJECTS = main.o map.o utils/interface.o utils/draw_line.o utils/polygons.o utils/pane.o utils/zoom.o utils/rotate.o utils/clipping.o utils/texture.o
OUTPUT = main

$(OUTPUT): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(OUTPUT) $(OBJECTS) $(LDLIBS)


main.o: main.c utils/mesa.h utils/utils.h utils/polygons.h utils/interface.h utils/coordinates.h
	$(CC) $(CFLAGS) -c main.c -o main.o


map.o: map.c utils/coordinates.h
	$(CC) $(CFLAGS) -c map.c -o map.o


utils/interface.o: utils/interface.c utils/interface.h utils/coordinates.h
	$(CC) $(CFLAGS) -c utils/interface.c -o utils/interface.o

utils/draw_line.o: utils/draw_line.c utils/draw_line.h
	$(CC) $(CFLAGS) -c utils/draw_line.c -o utils/draw_line.o

utils/polygons.o: utils/polygons.c utils/polygons.h utils/color.h utils/coordinates.h utils/interface.h utils/mesa.h utils/rotate.h utils/draw_line.h utils/texture.h
	$(CC) $(CFLAGS) -c utils/polygons.c -o utils/polygons.o

utils/pane.o: utils/pane.c utils/pane.h
	$(CC) $(CFLAGS) -c utils/pane.c -o utils/pane.o

utils/zoom.o: utils/zoom.c utils/zoom.h
	$(CC) $(CFLAGS) -c utils/zoom.c -o utils/zoom.o

utils/rotate.o: utils/rotate.c utils/rotate.h
	$(CC) $(CFLAGS) -c utils/rotate.c -o utils/rotate.o
	
utils/texture.o: utils/texture.c utils/texture.h
	$(CC) $(CFLAGS) -c utils/texture.c -o utils/texture.o

utils/clipping.o: utils/clipping.c utils/clipping.h
	$(CC) $(CFLAGS) -c utils/clipping.c -o utils/clipping.o
	
clean:
	rm -f *.o
	rm -f utils/*.o
	rm -f $(OUTPUT)
