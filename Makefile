TARGET := id100
CC := gcc
CFLAGS := -Ofast -flto=jobserver -Wall -fomit-frame-pointer
LFLAGS := -s

GIT_STATUS := $(shell git status --porcelain)
ifeq ($(strip $(GIT_STATUS)),)
DEFINES := -D GIT_HASH=\"$(shell git rev-parse --short=4 HEAD)\"
endif

SRCDIR := src
OBJDIR := obj
INSTALLDIR := /usr/local/bin
INSTALL := sudo install -o root -g root
RM := rm -rf
MKDIR := mkdir -p

.PHONY: default all clean remake install

default: $(TARGET)
all: default
remake: clean $(TARGET)

OBJECTS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.c))

-include $(OBJECTS:.o=.d)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(MKDIR) -p $(@D)
	+$(CC) $(DEFINES) $(CFLAGS) -MMD -c $< -o $@

$(TARGET): $(OBJECTS)
	+$(CC) $(CFLAGS) $(LFLAGS) $(OBJECTS) $(LIBS) -o $@

clean:
	$(RM) $(TARGET) $(OBJDIR)

install: $(TARGET)
	$(INSTALL) -s $(TARGET) $(INSTALLDIR)
