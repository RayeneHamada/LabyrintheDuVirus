#ifndef VIRUS_H
#define VIRUS_H

class virus {
  private:

    virus_state nature;
    direction dir;
    coord currentPos;
    double timer;

  public:

    // Virus speed in cases per seconds
    static double speed;

    // game's maze
    static Labyrinthe* maze;

    virus(coord, virus_state);

    // updates attached virus position according to its speed and direction
    void updatePosition(double);

    // updates direction randomly
    void updateDirection();

    /*--------------- getters -------------*/
    coord getPosition() const;
    virus_state getNature() const;
    /*-------------------------------------*/

    /*--------------- setters -------------*/
    void setPosition(coord);
    void setDirection(direction);
    void setNature(virus_state);
    /*-------------------------------------*/

};

#endif
