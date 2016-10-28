CC=clang
CFLAGS=-g -O0 -Wall -c
LDFLAGS=
SOURCEDIR=src/
OUTDIR=bin
SOURCES=$(wildcard $(SOURCEDIR)*.c)
OBJECTS=$(patsubst $(SOURCEDIR)%.c,$(OUTDIR)/%.o,$(SOURCES))
EXECUTABLE=hsh

all: $(OUTDIR)/$(EXECUTABLE)

$(OUTDIR)/$(EXECUTABLE): $(OBJECTS)
	    $(CC) $^ -o $@

$(OBJECTS): $(OUTDIR)/%.o : $(SOURCEDIR)%.c
	    $(CC) $(CFLAGS) $< -o $@

clean:
	    rm -f $(OUTDIR)/*o $(OUTDIR)/$(EXECUTABLE)
