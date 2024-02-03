CC=gcc
CFLAGS=-Wall -O3
SOURCES= test.c\
		pow.c
OBJECTS=$(SOURCES:.c=.o)

#Name of the executable file
EXECUTABLE=run_test

all: $(EXECUTABLE)

#$@ = $(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

# ".c.o" is a suffix rule telling make how to turn file.c into file.o
# for an arbitrary file.
#
# $< is an automatic variable referencing the source file,
# file.c in the case of the suffix rule.
#
# $@ is an automatic variable referencing the target file, file.o.
# file.o in the case of the suffix rule.
#
# Use -c to generatethe .o file
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm *.o $(EXECUTABLE)