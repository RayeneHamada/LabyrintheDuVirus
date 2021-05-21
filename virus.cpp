#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

struct coord {
  int x;
  int y;
};

enum virus_state {
  NO_VIRUS,
  WEAK_VIRUS,
  COVID19_VIRUS
};

enum direction {
  UP = 0,
  DOWN = 1,
  LEFT = 2,
  RIGHT = 3
};

enum potion_state {
  NO_POTION,
  ENERGETIC_POTION,
  DISINFECTANT_GEL
};


#include "labyrinthe.h"
#include "virus.h"

// creating static class members
double virus::speed;
Labyrinthe* virus::maze;

virus::virus(coord position, virus_state nature) : currentPos{position}, nature{nature}, timer{0} {
  updateDirection();  // sets initial direction randomly
}

virus_state virus::getNature() const {
  return nature;
}

void virus::updatePosition(double deltaTime) {
  timer += deltaTime;

  switch(dir) {
    case UP :       if ((*maze)(currentPos.x - 1, currentPos.y).state == WALL) {
                      updateDirection();
                    } else if ( static_cast<int>(timer*speed) != 0 ) {
                      --currentPos.x;
                      timer = 0;
                    }
                    break;

    case DOWN :     if ((*maze)(currentPos.x + 1, currentPos.y).state == WALL) {
                      updateDirection();
                    } else if (static_cast<int>(timer*speed) != 0) {
                      ++currentPos.x;
                      timer = 0;
                    }
                    break;

    case LEFT :     if ((*maze)(currentPos.x, currentPos.y - 1).state == WALL) {
                      updateDirection();
                    } else if (static_cast<int>(timer*speed) != 0) {
                      --currentPos.y;
                      timer = 0;
                    }
                    break;

    case RIGHT :    if ((*maze)(currentPos.x, currentPos.y + 1).state == WALL) {
                      updateDirection();
                    } else if (static_cast<int>(timer*speed) != 0) {
                      ++currentPos.y;
                      timer = 0;
                    }
                    break;

    default :       break;
  }
}

void virus::updateDirection() {

  bool possible_direction[] = {false, false, false, false};

  if ( (*maze)(currentPos.x-1, currentPos.y).state == PASS) {
    possible_direction[UP] = true;
  }

  if ( (*maze)(currentPos.x+1, currentPos.y).state == PASS) {
    possible_direction[DOWN] = true;
  }

  if ( (*maze)(currentPos.x, currentPos.y-1).state == PASS) {
    possible_direction[LEFT] = true;
  }

  if ( (*maze)(currentPos.x, currentPos.y+1).state == PASS) {
    possible_direction[RIGHT] = true;
  }

  int posDir;
  do {
    posDir = std::rand()/(RAND_MAX/4);
  } while (!possible_direction[posDir]);

  dir = static_cast<direction>(posDir);
}

coord virus::getPosition() const {
  return currentPos;
}
