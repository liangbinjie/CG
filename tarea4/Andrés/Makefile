CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_DEFAULT_SOURCE -I.

ifeq ($(OS),Windows_NT)
TARGET = main.exe
LDFLAGS = -lfreeglut -lopengl32 -lglu32 -lm
else
TARGET = main
LDFLAGS = -lglut -lGL -lGLU -lm
endif

SOURCES = main.c utils.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

main.o: main.c mesa.h utils.h
	$(CC) $(CFLAGS) -c $< -o $@

utils.o: utils.c mesa.h utils.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(OBJECTS) $(TARGET)
