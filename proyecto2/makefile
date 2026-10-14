CC = cc

OBJECTS = main.o utils/interface.o config/config.o object/object.o
OUTPUT = main

$(OUTPUT): $(OBJECTS)
	$(CC) -o $(OUTPUT) $(OBJECTS) $(LDLIBS)


main.o: main.c utils/utils.h utils/interface.h config/config.h object/object.h
	$(CC) $(CFLAGS) -c main.c -o main.o

interface.o: utils/interface.c utils/interface.h
	$(CC) $(CFLAGS) -c utils/interface.c -o utils/interface.o

config.o: config/config.c config/config.h
	$(CC) $(CFLAGS) -c config/config.c -o config/config.o

object.o: object/object.c object/object.h
	$(CC) $(CFLAGS) -c object/object.c -o object/object.o

clean:
	rm -f *.o
	rm -f utils/*.o
	rm -f $(OUTPUT)
