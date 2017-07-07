TARGET = id100
CC = gcc
CFLAGS = -Ofast -flto -Wall -fomit-frame-pointer
LIBS =
LFLAGS = -s
DEFINES = -D GIT_HASH=\"$(shell git rev-parse --short=4 HEAD)\"

SRCDIR = src
OBJDIR = obj
INSTALLDIR = /usr/local/bin
INSTALL = sudo install -o root -g root

.PHONY: default all clean remake install

default: $(TARGET)
all: default
remake: clean $(TARGET)

OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.c))

-include $(OBJECTS:.o=.d)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(DEFINES) $(CFLAGS) -MMD -c $< -o $@

$(TARGET): $(OBJDIR) $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJECTS) -Wall $(LIBS) -o $@

.PRECIOUS: $(TARGET) $(OBJECTS) $(OBJDIR)

clean:
	-rm -rf $(TARGET) $(OBJDIR)

install: $(TARGET)
	$(INSTALL) -s $(TARGET) $(INSTALLDIR)
