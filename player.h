#ifndef PLAYER_H
#define PLAYER_H

#define MAX_HP 12

enum character_type {
    MALE,
    FEMALE,
    CHILD,
    OTHER
};

class player {
  public:

    player(character_type, coord, direction, int);
    player();
    // updates player position according to its speed and its direction
    void updatePosition(double);

    /*--------------------------------- Getters -------------------------------*/
    int getHp() const;
    character_type getCharacter() const;
    coord getPosition() const;
    direction getDirection() const;
    /*-------------------------------------------------------------------------*/

    /*--------------------------------- Setters -------------------------------*/
    void setMaze(Labyrinthe*);
    void setSpeed(int);
    void setCharacter(character_type);
    void setStartPosition(coord);
    bool setDirection(direction);
    void setHp(int);
    /*-------------------------------------------------------------------------*/

    // speed in cases per second
    static double speed;

  private:
    character_type nature;
    int hp;
    coord currentPos;
    direction dir;

    // game's maze
    Labyrinthe* maze;

};

#endif
