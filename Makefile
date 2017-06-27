TARGET = id100
CC = gcc
CFLAGS = -Ofast -flto -Wall -fomit-frame-pointer
LIBS = 
LFLAGS = -s

INSTALLDIR = /usr/local/bin
INSTALL = sudo install -o root -g root

.PHONY: default all clean remake install

default: $(TARGET)
all: default
remake: clean $(TARGET)

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))

-include $(OBJECTS:.o=.d)

%.o: %.c
	$(CC) $(CFLAGS) -MMD -c $*.c -o $*.o

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f $(TARGET) *.o *.d

install: $(TARGET)
	$(INSTALL) -s $(TARGET) $(INSTALLDIR)
