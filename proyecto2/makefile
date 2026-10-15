CC = cc
LDLIBS = -lm
OBJECTS = main.o utils/interface.o utils/avs.o config/config.o object/object.o
OUTPUT = main

$(OUTPUT): $(OBJECTS)
	$(CC) -o $(OUTPUT) $(OBJECTS) $(LDLIBS)


main.o: main.c utils/utils.h utils/interface.h utils/avs.h config/config.h object/object.h
	$(CC) $(CFLAGS) -c main.c -o main.o

utils/interface.o: utils/interface.c utils/interface.h
	$(CC) $(CFLAGS) -c utils/interface.c -o utils/interface.o

utils/avs.o: utils/avs.c utils/avs.h config/config.h
	$(CC) $(CFLAGS) -c utils/avs.c -o utils/avs.o

config/config.o: config/config.c config/config.h object/object.h
	$(CC) $(CFLAGS) -c config/config.c -o config/config.o

object/object.o: object/object.c object/object.h config/config.h
	$(CC) $(CFLAGS) -c object/object.c -o object/object.o

clean:
	rm -f *.o
	rm -f utils/*.o
	rm -f config/*.o
	rm -f object/*.o
	rm -f $(OUTPUT)
