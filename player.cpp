#include <iostream>
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

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdexcept>

#include "labyrinthe.h"
#include "player.h"

// create static (global) variable
double player::speed;

player::player(character_type nature, coord position, direction dir, int speed) : nature{nature}, hp{10} {
  if ( (position.x >= maze->getCols()) || (position.y >= maze->getRows()) ) {
    throw std::out_of_range("Chosen coordinates are not in maze!");
  } else if ((*maze)(currentPos.x, currentPos.y).state == WALL) {
    // some exception here
  } else {
    currentPos = position;
  }

  if (!setDirection(dir)) {
    // some exception here
  }
}

player::player() : hp{10} {}

void player::updatePosition(double deltaTime) {
  static double timer = 0;
  timer += deltaTime;
  switch (dir) {
    case UP :       if ((*maze)(currentPos.x - 1, currentPos.y).state == WALL) {
                      dir = DOWN;   // reverse direction
                    } else if ( static_cast<int>(timer*speed) != 0 ) {
                      --currentPos.x;
                      timer = 0;
                    }
                    break;

    case DOWN :     if ((*maze)(currentPos.x + 1, currentPos.y).state == WALL) {
                      dir = UP;
                    } else if (static_cast<int>(timer*speed) != 0) {
                      ++currentPos.x;
                      timer = 0;
                    }
                    break;

    case LEFT :     if ((*maze)(currentPos.x, currentPos.y - 1).state == WALL) {
                      dir = RIGHT;
                    } else if (static_cast<int>(timer*speed) != 0) {
                      --currentPos.y;
                      timer = 0;
                    }
                    break;

    case RIGHT :    if ((*maze)(currentPos.x, currentPos.y + 1).state == WALL) {
                      dir = LEFT;
                    } else if (static_cast<int>(timer*speed) != 0) {
                      ++currentPos.y;
                      timer = 0;
                    }
                    break;

    default :       break;
  }
}

// the principle here is : if you can't keep going this direction, go the opposite direction!
bool player::setDirection(direction pDir) {

  switch (pDir) {
    case UP :     if ((*maze)(currentPos.x - 1, currentPos.y).state == PASS) {
                    dir = UP;
                  } else {
                    return false;
                  }
                  break;

    case DOWN :   if ((*maze)(currentPos.x + 1, currentPos.y).state == PASS) {
                    dir = DOWN;
                  } else {
                    return false;
                  }
                  break;

    case LEFT :   if ((*maze)(currentPos.x, currentPos.y - 1).state == PASS) {
                    dir = LEFT;
                  } else {
                    return false;
                  }
                  break;

    case RIGHT :  if((*maze)(currentPos.x, currentPos.y + 1).state == PASS) {
                    dir = RIGHT;
                  } else {
                    return false;
                  }
                  break;

    default :     return false;
                  break;
  }

return true;
}

void player::setSpeed(int pSpeed) {
  speed = pSpeed;
}

void player::setStartPosition(coord startPosition) {
  currentPos = startPosition;
}

void player::setCharacter(character_type character) {
  nature = character;
}

int player::getHp() const {
  return hp;
}

direction player::getDirection() const {
  return dir;
}

character_type player::getCharacter() const {
  return nature;
}

coord player::getPosition() const {
  return currentPos;
}

void player::setMaze(Labyrinthe* maze) {
  this->maze = maze;
}

void player::setHp(int healthPoints) {
  hp = healthPoints;
}
