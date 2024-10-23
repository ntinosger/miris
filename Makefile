# Use shell
SHELL = /bin/sh

# Object files to be compiled
OBJS = main.o node.o edge.o graph.o

# Compiler flags
CFLAGS = -Wall -g  # Enables warnings and debugging info

# Compiler to be used
CC = gcc

# Include directories (add any as necessary)
# INCLUDES =

# # Libraries to link with (add libraries as necessary)
# LIBS = -lm  # Math library

# Name of the executable
TARGET = miris

# Build the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJS) $(LIBS)

# Clean up object files and the executable
clean:
	-rm -f *.o core *.core $(TARGET)

# Compile all .c files into .o files
.c.o:
	$(CC) $(CFLAGS) -c $<

# Dependencies (optional: add header files if necessary)
main.o: main.c graph.h

