CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -g

# Default target
puzzle: ppm_io.o puzzle.o puzzle_funcs.o
	$(CC) ppm_io.o puzzle.o puzzle_funcs.o -o puzzle

# Target for testing
test: puzzle_tests.o puzzle_funcs.o ppm_io.o
	$(CC) puzzle_tests.o puzzle_funcs.o ppm_io.o -o test

# Object files for the puzzle executable
ppm_io.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm_io.c

puzzle.o: puzzle.c puzzle.h ppm_io.h
	$(CC) $(CFLAGS) -c puzzle.c

puzzle_funcs.o: puzzle_funcs.c puzzle.h ppm_io.h
	$(CC) $(CFLAGS) -c puzzle_funcs.c

# Object file for the test program
puzzle_tests.o: puzzle_tests.c puzzle.h ppm_io.h
	$(CC) $(CFLAGS) -c puzzle_tests.c

# Clean up
clean:
	rm -f *~ *.o puzzle test