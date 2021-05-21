#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>

#include "SDL_FUNCs.h"

struct coord {
  int x;
  int y;
};

enum virus_state {
  NO_VIRUS,
  WEAK_VIRUS,
  COVID19_VIRUS
};

enum potion_state {
  NO_POTION,
  ENERGETIC_POTION,
  DISINFECTANT_GEL
};

#include "labyrinthe.h"

Labyrinthe::Labyrinthe( int rows_param, int cols_param, coord start_point = {0, 0}, bool verbose = false,
                        SDL_Renderer* pRenderer = NULL) {

  /*--------------verification of rows, cols and start_point----------------*/
  // minimum size of maze is 15-15
  if (rows_param < 15 || cols_param < 15) {
    throw std::domain_error{"@Labyrinthe constructor"};
  }
  // rows and cols must be odd (just to avoid confusion)
  else {
    if (rows_param & 1) {
      rows = rows_param;
    } else {
      rows = rows_param+1;
    }

    if (cols_param & 1) {
      cols = cols_param;
    } else {
      cols = cols_param+1;
    }
  }

  if (  (start_point.x < 0 || start_point.x >= rows) ||
        (start_point.y < 0 || start_point.y >= cols)  ) {
    throw std::domain_error{"coord should be accessible through maze!"};
  }
  /*------------------------------------------------------------------------*/

  /*-------------------------- Member initialization -----------------------*/
  mRenderer = pRenderer;

  int w, h;
  SDL_GetRendererOutputSize(pRenderer, &w, &h);
  case_size = h/(rows_param+2);

  if ( (case_size*cols_param) > (w-200)) {
    // NOPE... YOU ARE NOT ALLOWED !
  }
  /*------------------------------------------------------------------------*/

  /*-----------------memory allocation for 2D maze array--------------------*/
  maze_array  = new maze_case*[rows];
  for (int i=0; i<rows; ++i) {
    maze_array[i] = new maze_case[cols];
  }
  /*------------------------------------------------------------------------*/

  /*--------------------Initialisation of the 2D array----------------------*/
  for (int i=0; i<rows; ++i) {
    if (i & 1) continue;
    for (int j=0; j<cols; ++j) {
      if (j & 1) continue;
      maze_array[i][j].state = PASS;
    }
  }
  /*------------------------------------------------------------------------*/

  if (verbose) {
    renderMaze({0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00});
  }

  /*---------------------algorithm initialisations--------------------------*/
  // stack that holds the history of previously gone-through cases
  std::vector<coord> case_stack;
  int nb_visited_cases = 1;

  // add starting point to the top of the stack
  case_stack.push_back(start_point);

  // of course, we start by visiting the first starting point
  maze_array[start_point.x][start_point.y].visited = true;
  /*------------------------------------------------------------------------*/

  /*------------------recursive backtracking algorithm----------------------*/
  std::srand(std::time(nullptr));
  while (nb_visited_cases != ((rows/2)+1) * ((cols/2)+1) ) {
    coord cur_pos = case_stack.back();

    //                         up     down   left   right
    bool possible_direct[4] = {false, false, false, false};
    bool no_possible_direction = true;

    // updatig possible_direct array
    if ((cur_pos.x-2 >= 0) && (!maze_array[cur_pos.x-2][cur_pos.y].visited)) {
      possible_direct[0] = true;
      no_possible_direction = false;
    }


    if ((cur_pos.x+2 < rows) && (!maze_array[cur_pos.x+2][cur_pos.y].visited)) {
      possible_direct[1] = true;
      no_possible_direction = false;
    }


    if ((cur_pos.y-2 >= 0) && (!maze_array[cur_pos.x][cur_pos.y-2].visited)) {
      possible_direct[2] = true;
      no_possible_direction = false;
    }


    if ((cur_pos.y+2 < cols) && (!maze_array[cur_pos.x][cur_pos.y+2].visited)) {
      possible_direct[3] = true;
      no_possible_direction = false;
    }


    // if all neighbor cases are inaccessible
    if (no_possible_direction) {

        // check history and go back one step and do the process of choosing -if possible-
        // a neighbor case
        case_stack.pop_back();

    } else {

      // choose a random possible direction
      int direction;

      do {
        direction = std::rand()/(RAND_MAX/4);
      } while (!possible_direct[direction]);

      // add the new accessed case to the top of the stack and
      // remove wall between passages (between cur_pos and the new chosen position)
      switch (direction) {

        case 0 :  maze_array[cur_pos.x-2][cur_pos.y].visited = true;
                  case_stack.push_back({cur_pos.x-2, cur_pos.y});
                  maze_array[cur_pos.x-1][cur_pos.y].state = PASS;
                  break;

        case 1 :  maze_array[cur_pos.x+2][cur_pos.y].visited = true;
                  case_stack.push_back({cur_pos.x+2, cur_pos.y});
                  maze_array[cur_pos.x+1][cur_pos.y].state = PASS;
                  break;

        case 2 :  maze_array[cur_pos.x][cur_pos.y-2].visited = true;
                  case_stack.push_back({cur_pos.x, cur_pos.y-2});
                  maze_array[cur_pos.x][cur_pos.y-1].state = PASS;
                  break;

        case 3 :  maze_array[cur_pos.x][cur_pos.y+2].visited = true;
                  case_stack.push_back({cur_pos.x, cur_pos.y+2});
                  maze_array[cur_pos.x][cur_pos.y+1].state = PASS;
                  break;
      }

      // due to the fact that we've just visited a newly, unvisited case
      ++nb_visited_cases;

    } // END IF
  } // END WHILE
  /*------------------------------------------------------------------------*/

}

Labyrinthe::~Labyrinthe() {
  for (int i=0; i<rows; ++i) {
    delete [] maze_array[i];
  }
  delete [] maze_array;
}

void Labyrinthe::show_maze(void) {
  for (int i=0; i<rows; ++i) {
    for (int j=0; j<cols; j++) {
      if (maze_array[i][j].state == WALL)
        std::cout << "#";
      else
        std::cout << ".";
    }
    std::cout << std::endl;
  }
}

std::ostream& operator<<(std::ostream& os, coord point) {
  os << "(" << point.x << " ," << point.y << ")";
  return os;
}

int Labyrinthe::getRows(void) const {return rows;}
int Labyrinthe::getCols(void) const {return cols;}

int Labyrinthe::getCaseSize() const {return case_size;}

coord Labyrinthe::getStartPoint(void) const {return start_point;}
coord Labyrinthe::getEndPoint(void) const {return end_point;}

void Labyrinthe::setStartPoint(coord start_point) {
  if (  (start_point.x < 0 || start_point.x >= rows) ||
        (start_point.y < 0 || start_point.y >= cols)  ) {
    throw std::domain_error{"Starting point should be accessible through maze!"};
  } else if (maze_array[start_point.x][start_point.y].state == WALL) {
    // handle exception here
  } else {
    this->start_point = start_point;
  }
}
void Labyrinthe::setEndPoint(coord point) {end_point = point;}

void Labyrinthe::setCaseState(int x, int y, maze_state pState = PASS) {
  maze_array[x][y].state = pState;
}

void Labyrinthe::setVirus(int x, int y, virus_state pVirus) {
  maze_array[x][y].virus = pVirus;
}

void Labyrinthe::setPotion(int x, int y, potion_state pPotion) {
  maze_array[x][y].potion = pPotion;
}

maze_case Labyrinthe::operator() (int i, int j) {
  if ( (i > rows) || (j > cols) ) throw std::out_of_range{"coordonates provided are out maze!"};
  if ( (i == rows || i == -1) || (j == cols || j == -1)) return maze_case{};
  return maze_array[i][j];
}

void Labyrinthe::add_random_rooms(int nb_rooms) {

  if (nb_rooms > 4) throw std::domain_error{"Maximum 4 rooms!"};

  int nb_room_created = 0;
  while (nb_room_created != nb_rooms) {

    int room_size_width = 3 + std::rand()/(RAND_MAX/7);
    int room_size_height = 3 + std::rand()/(RAND_MAX/7);

    // rand_pos reflects top-left block of the room block
    // generate rand_pos out of all possible positions in the maze
    coord rand_pos {  std::rand()/( RAND_MAX/(rows-room_size_height+1) ),
                      std::rand()/( RAND_MAX/(cols-room_size_width+1) )   };

    for (int i=rand_pos.x; i<rand_pos.x+room_size_height; ++i)
      for (int j=rand_pos.y; j<rand_pos.y+room_size_width; ++j)
        if (maze_array[i][j].state == WALL) maze_array[i][j].state = PASS;

    ++nb_room_created;
  } // END WHILE

} // END FUNCTION

// Renderes the current state of the maze to the renderer
void Labyrinthe::renderMaze(SDL_Color pass_color, SDL_Color wall_color) {

  SDL_Rect currRect{case_size, case_size, case_size, case_size};

  for (int i=0; i<rows; ++i) {
    for (int j=0; j<cols; ++j) {

      if (maze_array[i][j].state == PASS) {

        SDL_SetRenderDrawColor(mRenderer, pass_color.r, pass_color.g, pass_color.b, pass_color.a);
        SDL_RenderFillRect(mRenderer, &currRect);

      } else {

        SDL_SetRenderDrawColor(mRenderer, wall_color.r, wall_color.g, wall_color.b, wall_color.a);
        SDL_RenderFillRect(mRenderer, &currRect);
      }


      currRect.x += case_size;
    } // END NESTED FOR

  currRect.x = case_size;
  currRect.y += case_size;

  }

  // Rendering surrounding wall
  SDL_SetRenderDrawColor(mRenderer, wall_color.r, wall_color.g, wall_color.b, wall_color.a);

  currRect = {0, 0, case_size*(cols+2), case_size};   // upper wall
  SDL_RenderFillRect(mRenderer, &currRect);

  currRect = {0, case_size*(rows+1), case_size*(cols+2), case_size}; // lower wall
  SDL_RenderFillRect(mRenderer, &currRect);

  currRect = {0, case_size, case_size, case_size*rows}; // left wall
  SDL_RenderFillRect(mRenderer, &currRect);

  currRect = {case_size*(cols+1), case_size, case_size, case_size*rows}; // right wall
  SDL_RenderFillRect(mRenderer, &currRect);
}
