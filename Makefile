TARGET = id100
CC = gcc
CFLAGS = -Ofast -flto -Wall -fomit-frame-pointer
LIBS = 
LFLAGS = -s

INSTALLDIR = /opt/fhem
INSTALL = sudo install -m 4755 -o root -g root

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)

install: $(TARGET)
	$(INSTALL) -s $(TARGET) $(INSTALLDIR)
