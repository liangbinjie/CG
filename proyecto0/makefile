CC = gcc
CFLAGS = -Wall -Wextra -g

OUTPUT_NAME = main

SRC = main.c utils.c

$(OUTPUT_NAME): $(SRC)
	$(CC) $(CFLAGS) -o $(OUTPUT_NAME) $(SRC)

clean:
	rm -f $(OUTPUT_NAME)