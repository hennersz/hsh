CC=gcc
CFLAGS= -Wall -c
LDFLAGS=
SOURCEDIR=src/
OUTDIR=bin
SOURCES=$(wildcard $(SOURCEDIR)*.c)
OBJECTS=$(patsubst $(SOURCEDIR)%.c,$(OUTDIR)/%.o,$(SOURCES))
EXECUTABLE=shell

all: ./$(EXECUTABLE)

./$(EXECUTABLE): $(OBJECTS)
	    $(CC) $^ -o $@

$(OBJECTS): $(OUTDIR)/%.o : $(SOURCEDIR)%.c
	    $(CC) $(CFLAGS) $< -o $@

clean:
	    rm -f $(OUTDIR)/*o ./$(EXECUTABLE)
