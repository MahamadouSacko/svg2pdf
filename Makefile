EXEC=svg2pdf.exe
SOURCES=xml.c pdf.c svg.c args.c menu.c main.c
OBJECTS=$(SOURCES:.c=.o)
CC=gcc
CFLAGS=-W -Wall 
LDLIBS= -lm 
.PHONY: default clean
 
default: $(EXEC)
 
xml.o: xml.c xml.h
pdf.o: pdf.c pdf.h xml.h
svg.o: svg.c xml.h pdf.h svg.h -lm
menu.o: menu.c menu.h
args.o: args.c args.h
main.o: main.c svg.h args.h menu.h 

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
 
$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^ $(LDLIBS)
 
clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~

