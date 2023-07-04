CFLAGS = -Wall -MMD
LDFLAGS = -O
LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system

PROGRAM = pacman.exe
OBJECTS = screen.o textbox.o pacman.o env.o main.o

$(PROGRAM): $(OBJECTS)
	g++ $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $(PROGRAM)

H_LINK := $(OBJECTS:.o=.d)

.PHONY: run del

run: $(PROGRAM)
	@./$(PROGRAM)

del:
	-rm -rf $(OBJECTS) $(H_LINK) $(PROGRAM)

-include $(H_LINK)