CC = gcc
CCFLAGS = -Wall

SRCDIR = src
BINDIR = bin

# All source and header files found within src/ with corresponding object files for .c files
SRCFILES = $(wildcard $(SRCDIR)/*.c)
OBJFILES = $(SRCFILES:$(SRCDIR)/%.c=$(BINDIR)/%.o)
EXECUTABLE = $(BINDIR)/super-ttt
HEADFILES = $(wildcard $(SRCDIR)/*.h)

all: $(EXECUTABLE)

# Appends appropriate debug information and optimization flags for debug and release modes
debug: CCFLAGS += -O0 -g
debug: $(EXECUTABLE)

release: CCFLAGS += -O2
release: $(EXECUTABLE)

$(EXECUTABLE): $(OBJFILES)
	$(CC) $(CCFLAGS) -lpthread -o $(EXECUTABLE) $(OBJFILES)

$(BINDIR)/%.o: $(SRCDIR)/%.c $(HEADFILES)
	$(CC) $(CCFLAGS) -c -o $@ $<

.phony: all debug release