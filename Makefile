CC = gcc
CFLAGS = -Wall -Iheaders
LDFLAGS = -lncursesw
TARGET = Tetris
SOURCEDIR = sources
HEADERSDIR = headers
SOURCES = $(wildcard $(SOURCEDIR)/*.c) Tetris.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean

all: $(TARGET) clean
	@echo "Binary ready !"

$(TARGET): $(OBJECTS)
	@echo "Compiling and linking $(TARGET)..."
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c $(HEADERSDIR)/*.h
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning..."
	rm -f $(OBJECTS)
