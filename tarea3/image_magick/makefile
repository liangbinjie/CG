CC=cc


OBJECTS=main.o
OUTPUT=main

$(OUTPUT):$(OBJECTS)
	$(CC) -o $(OUTPUT) $(OBJECTS)

main.o: main.c 

clean:
	rm -f *.o
	rm -f $(OUTPUT)
