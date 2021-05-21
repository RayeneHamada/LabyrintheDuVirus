#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

struct coord {
  int x;
  int y;
};

enum potion_state {
  NO_POTION,
  ENERGETIC_POTION,
  DISINFECTANT_GEL
};

enum virus_state {
  NO_VIRUS,
  WEAK_VIRUS,
  COVID19_VIRUS
};

enum direction {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

#include "labyrinthe.h"
#include "potion.h"

// creating potion's static object
Labyrinthe* potion::maze;

potion::potion(coord position, potion_state nature) : currentPos{position}, nature{nature} {}

potion_state potion::getNature() const {
  return nature;
}

coord potion::getPosition() const {
  return currentPos;
}
