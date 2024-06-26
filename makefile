CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.c game.c utils.c mouse.c keyboard.c manymouse/linux_evdev.c manymouse/manymouse.c -lX11
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=app

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

run: all
	./$(EXECUTABLE)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
