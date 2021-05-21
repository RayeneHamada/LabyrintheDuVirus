#ifndef LABYRINTHE_H
#define LABYRINTHE_H

enum maze_state {
  WALL,
  PASS
};

struct maze_case {
  maze_state state = WALL;          // set all the maze_array's case by default to WALL
  bool visited = false;             // usefull for recursive backtracking algorithm
  virus_state virus = NO_VIRUS;     // is there a virus here ?
  potion_state potion = NO_POTION;  // is there a potion here ?
};


class Labyrinthe {
private:
  coord start_point;
  coord end_point;
  maze_case** maze_array;
  int rows;
  int cols;

  int case_size = 0;                 // size of a case in the maze in pixels
  SDL_Renderer* mRenderer = NULL;    // rendering context
public:
  Labyrinthe(int, int, coord, bool, SDL_Renderer*);

  // renders the current maze
  void renderMaze(SDL_Color, SDL_Color);

  /*--------------------------------getters---------------------------------*/
  int getRows() const;
  int getCols() const;

  int getCaseSize() const;

  coord getStartPoint() const;
  coord getEndPoint() const;
  /*------------------------------------------------------------------------*/

  /*-------------------------------setters----------------------------------*/
  void setCaseState(int, int, maze_state);
  void setPotion(int, int, potion_state);
  void setVirus(int, int, virus_state);

  void setStartPoint(coord);
  void setEndPoint(coord);
  void show_maze(void);       // outputs maze's shape to output stream
  /*------------------------------------------------------------------------*/

  // returns only an rvalue, used for readonly
  maze_case operator() (int, int);

  void add_random_rooms(int);

  ~Labyrinthe();
};

#endif
