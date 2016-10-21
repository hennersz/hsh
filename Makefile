CC=clang
CFLAGS=-g -O0 -Wall

SOURCEDIR = src/
OUTDIR = bin/
TARGET = hsh
RM = rm -r

all: $(TARGET)

$(TARGET): $(SOURCEDIR)$(TARGET).c
	$(CC) $(CFLAGS) -o $(OUTDIR)$(TARGET) $(SOURCEDIR)$(TARGET).c

clean:
	$(RM) $(OUTDIR)*
