CC = gcc
CFLAGS = -Wall --std=c99 -g
OBJECTS = main.o my_string.o

string_driver: $(OBJECTS)
	$(CC) $(CFLAGS) -o string_driver $(OBJECTS)
main.o: main.c my_string.h generic.h
	$(CC) $(CFLAGS) -c main.c -o main.o
my_string.o: my_string.h my_string.c generic.h
	$(CC) $(CFLAGS) -c my_string.c -o my_string.o
clean:
	rm string_driver $(OBJECTS)

