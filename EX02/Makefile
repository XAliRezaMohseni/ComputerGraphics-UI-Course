# Author: AliReza Mohseni al[dot]mohseni[at]yahoo[dot]com


CC = gcc
LDFLAGS = -lGL -lX11 -lglut -lm
CFLAGS  = -g -Wall -Wextra -ggdb
SOURCES = main.c sources/DisplayManager.c
OBJECTS = $(SOURCES:.c=.o)
TARGET  = 002

.PHONY: all, clean

.SILENT:

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

%.o:%.c
	gcc $(CFLAGS) $(LDFLAGS) -c $< -o $@

claen:
	rm $(OBJECTS) $(TARGET)
