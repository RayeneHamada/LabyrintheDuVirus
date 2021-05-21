#ifndef POTION_H
#define POTION_H

class potion {
  public:

    // game's maze
    static Labyrinthe* maze;

    potion(coord, potion_state);

    /*---------------- Gettters -------------*/
    potion_state getNature() const;
    coord getPosition() const;
    /*---------------------------------------*/

  private:
    potion_state nature;
    coord currentPos;
};

#endif
