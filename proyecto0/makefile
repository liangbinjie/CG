CC = cc
CFLAGS = -I/usr/local/Mesa-3.4/include
LDLIBS=-lX11 -lglut -lGLU -lGL -lm -lXext -lXmu
LDFLAGS=-L/usr/local/Mesa-3.4/lib -L/usr/X11R6/lib
NASM = nasm

OBJECTS = main.o utils.o bressenham_line.o
OUTPUT = main

$(OUTPUT): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(OUTPUT) $(OBJECTS) $(LDLIBS)

main.o: main.c mesa.h utils.h

utils.o: utils.c utils.h

bressenham_line.o: bressenham_line.asm
	$(NASM) -f elf64 bressenham_line.asm -o bressenham_line.o  

clean:
	rm -f *.o
	rm -f $(OUTPUT)
