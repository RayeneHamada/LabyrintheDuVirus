#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

game : game.o labyrinthe.o player.o virus.o potion.o
	g++ game.o labyrinthe.o player.o virus.o potion.o $(LINKER_FLAGS) -o game

game.o : game.cpp SDL_FUNCs.h labyrinthe.h virus.h potion.h player.h
	g++ -g -c game.cpp

labyrinthe.o : labyrinthe.cpp SDL_FUNCs.h labyrinthe.h
	g++ -g -c labyrinthe.cpp

player.o : player.cpp labyrinthe.h player.h
	g++ -g -c player.cpp

virus.o : virus.cpp labyrinthe.h virus.h
	g++ -g -c virus.cpp

potion.o : potion.cpp labyrinthe.h potion.h
	g++ -g -c potion.cpp

.PHONY = clean

clean : 
	rm game *.o
