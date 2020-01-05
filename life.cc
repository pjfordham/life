#include <string.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>

#include <iostream>

const int BOARD_SIZE = 100;
const float TILE_SIZE = 10.0f;

#define HEIGHT BOARD_SIZE
#define WIDTH BOARD_SIZE

enum content_t {
    Snake, Food, Empty, Head };

struct Shape {
public:
    int height;
    int width;
    const char * const* figure;
};

const char *crab[] = {
   "....X.....X....",
   "....X.....X....",
   "....XX...XX....",
   "...............",
   "XXX..XX.XX..XXX",
   "..X.X.X.X.X.X..",
   "....XX...XX....",
   "...............",
   "....XX...XX....",
   "..X.X.X.X.X.X..",
   "XXX..XX.XX..XXX",
   "...............",
   "....XX...XX....",
   "....X.....X....",
   "....X.....X...." };

struct Crab : public Shape {
   Crab() {
      figure = crab;
      height = sizeof( crab ) / sizeof( *crab );
      width = strlen( crab[0] );
   }
};

const char *rpentomino[] = {
   ".XX",
   "XX.",
   ".X." };

struct RPentomino : public Shape {
   RPentomino() {
      figure = rpentomino;
      height = sizeof( rpentomino ) / sizeof( *rpentomino );
      width = strlen( rpentomino[0] );
   }
};

const char *glider[] = {
   ".X.",
   "..X",
   "XXX" };

struct Glider : public Shape {
   Glider() {
      figure = glider;
      height = sizeof( glider ) / sizeof( *glider );
      width = strlen( glider[0] );
   }
};

const char *blinker[] = {
   "XXX" };

struct Blinker : public Shape {
   Blinker() {
      figure = blinker;
      height = sizeof( blinker ) / sizeof( *blinker );
      width = strlen( blinker[0] );
   }
};

const char *almond[] = {
   ".X.",
   "X.X",
   "X.X",
   ".X." };

struct Almond : public Shape {
   Almond() {
      figure = almond;
      height = sizeof( almond ) / sizeof( *almond );
      width = strlen( almond[0] );
   }
};

const char *spaceship[] = {
   "X...X.",
   ".....X",
   "X....X",
   ".XXXXX" };

struct SpaceShip : public Shape {
   SpaceShip() {
      figure = spaceship;
      height = sizeof( spaceship ) / sizeof( *spaceship );
      width = strlen( spaceship[0] );
   }
};

class GameOfLife {
public:
   GameOfLife();
   void addShape( Shape shape );

   void print();
   void update();
   void clear();
   content_t getContent( int i, int j);
   char getState( char state , int xCoord , int yCoord , bool toggle);
   void iterate(unsigned int iterations);
private:
   char world[HEIGHT][WIDTH];
   char otherWorld[HEIGHT][WIDTH];
   bool toggle;
   std::random_device rd;
   std::mt19937 randomNumbers;
};

GameOfLife::GameOfLife() :
   toggle(true), randomNumbers(rd())
{
   for ( int i = 0; i < HEIGHT; i++ ) {
      for ( int j = 0; j < WIDTH; j++ ) {
         world[i][j] = '.';
      }
   }
}

void GameOfLife::clear() {
   toggle = true;
   for ( int i = 0; i < HEIGHT; i++ ) {
      for ( int j = 0; j < WIDTH; j++ ) {
         world[i][j] = '.';
      }
   }
}

void GameOfLife::addShape( Shape shape )
{
   std::uniform_int_distribution<int> randomXLocationRange(0, BOARD_SIZE-shape.width);
   std::uniform_int_distribution<int> randomYLocationRange(0, BOARD_SIZE-shape.height);

   int xCoord = randomXLocationRange( randomNumbers );
   int yCoord = randomYLocationRange( randomNumbers );
   for ( int i = yCoord; i - yCoord < shape.height; i++ ) {
      for ( int j = xCoord; j - xCoord < shape.width; j++ ) {
         if ( i < HEIGHT && j < WIDTH ) {
            if ( toggle ) {
               world[i][j] =
                  shape.figure[ i - yCoord ][j - xCoord ];
            } else {
               otherWorld[i][j] =
                  shape.figure[ i - yCoord ][j - xCoord ];
            }
         }
      }
   }
}

void GameOfLife::print() {
    if ( toggle ) {
        for ( int i = 0; i < HEIGHT; i++ ) {
            for ( int j = 0; j < WIDTH; j++ ) {
                std::cout << world[i][j];
            }
            std::cout << std::endl;
        }
    } else {
        for ( int i = 0; i < HEIGHT; i++ ) {
            for ( int j = 0; j < WIDTH; j++ ) {
                std::cout << otherWorld[i][j];
            }
            std::cout << std::endl;
        }
    }
    for ( int i = 0; i < WIDTH; i++ ) {
        std::cout << '=';
    }
    std::cout << std::endl;
}

content_t GameOfLife::getContent(int i, int j) {
   int content;
   if ( toggle ) {
      content = world[i][j];
   } else {
      content = otherWorld[i][j];
   }
   switch (content) {
   case 'X': return Head;
   default:
      return Empty;
   }
}

void GameOfLife::update() {
    if (toggle) {
        for ( int i = 0; i < HEIGHT; i++ ) {
            for ( int j = 0; j < WIDTH; j++ ) {
                otherWorld[i][j] =
                    GameOfLife::getState(world[i][j] , i , j , toggle);
            }
        }
        toggle = !toggle;
    } else {
        for ( int i = 0; i < HEIGHT; i++ ) {
            for ( int j = 0; j < WIDTH; j++ ) {
                world[i][j] =
                    GameOfLife::getState(otherWorld[i][j] , i , j , toggle);
            }
        }
        toggle = !toggle;
    }
}

char GameOfLife::getState( char state, int yCoord, int xCoord, bool toggle ) {
    char neighbors = 0;
    if ( toggle ) {
        for ( int i = yCoord - 1; i <= yCoord + 1; i++ ) {
            for ( int j = xCoord - 1; j <= xCoord + 1; j++ ) {
                if ( i == yCoord && j == xCoord ) {
                    continue;
                }
                if ( i > -1 && i < HEIGHT && j > -1 && j < WIDTH ) {
                    if ( world[i][j] == 'X' ) {
                        neighbors++;
                    }
                }
            }
        }
    } else {
        for ( int i = yCoord - 1; i <= yCoord + 1; i++ ) {
            for ( int j = xCoord - 1; j <= xCoord + 1; j++ ) {
                if ( i == yCoord && j == xCoord ) {
                    continue;
                }
                if ( i > -1 && i < HEIGHT && j > -1 && j < WIDTH ) {
                    if ( otherWorld[i][j] == 'X' ) {
                        neighbors++;
                    }
                }
            }
        }
    }
    if (state == 'X') {
        return ( neighbors > 1 && neighbors < 4 ) ? 'X' : '.';
    }
    else {
        return ( neighbors == 3 ) ? 'X' : '.';
    }
}

void GameOfLife::iterate( unsigned int iterations ) {
    for ( int i = 0; i < iterations; i++ ) {
        update();
    }
}


int main()
{
  sf::RenderWindow window(sf::VideoMode((2+BOARD_SIZE) * (int)TILE_SIZE, (2+BOARD_SIZE) * (int)TILE_SIZE), "Game of Life");

  GameOfLife gol;

  sf::Clock clock;

  bool running = false;
  while (window.isOpen()) {
     if ( running ) {
        sf::Time elapsed = clock.getElapsedTime();
        if (elapsed.asSeconds() > 0.05f) {
           gol.iterate(1);
           clock.restart();
        }
     }

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
      // Respond to key pressed events
      if (event.type == sf::Event::KeyPressed){
        if (event.key.code == sf::Keyboard::Escape){
          return 0;
        }
        if (event.key.code == sf::Keyboard::A){
           gol.addShape(Almond());
        }
        if (event.key.code == sf::Keyboard::S){
           gol.addShape(SpaceShip());
        }
        if (event.key.code == sf::Keyboard::G){
           gol.addShape(Glider());
        }
        if (event.key.code == sf::Keyboard::K){
           gol.addShape(Crab());
        }
        if (event.key.code == sf::Keyboard::R){
           gol.addShape(RPentomino());
        }
        if (event.key.code == sf::Keyboard::B){
           gol.addShape(Blinker());
        }
        if (event.key.code == sf::Keyboard::I){
           gol.iterate(1);
        }
        if (event.key.code == sf::Keyboard::P){
           gol.print();
        }
        if (event.key.code == sf::Keyboard::C){
           gol.clear();
        }
        if (event.key.code == sf::Keyboard::Space){
           running = ! running;
        }
        if (event.key.code == sf::Keyboard::Left){
          // game.setDirection( Game::Left );
        }
        if (event.key.code == sf::Keyboard::Right){
          // game.setDirection( Game::Right );
        }
        if (event.key.code == sf::Keyboard::Up){
          // game.setDirection( Game::Up );
        }
        if (event.key.code == sf::Keyboard::Down){
          // game.setDirection( Game::Down );
        }

      }
    }

    // Clear window to Blue to do blue boarder.
    window.clear( sf::Color::Blue );

    // draw black background for theatre of life
    sf::RectangleShape shape(sf::Vector2f(TILE_SIZE*BOARD_SIZE, TILE_SIZE*BOARD_SIZE));
    shape.setPosition( TILE_SIZE, TILE_SIZE);
    shape.setFillColor(sf::Color::Black);
    window.draw(shape);

    for( int x=0;x<BOARD_SIZE;x++ ){
      for ( int y = 0;y<BOARD_SIZE;y++) {
         switch (gol.getContent(x, y)) {
         case Empty:
            // Do nothing
            break;
         case Food:
         {
            sf::RectangleShape shape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            shape.setPosition((y+1)*TILE_SIZE, (x+1)*TILE_SIZE);
            shape.setFillColor(sf::Color::Red);
            window.draw(shape);
         }
         break;
         case Snake:
         {
            sf::RectangleShape shape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            shape.setPosition((y+1)*TILE_SIZE, (x+1)*TILE_SIZE);
            shape.setFillColor(sf::Color::White);
            window.draw(shape);
         }
         break;
         case Head:
         {
            sf::RectangleShape shape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            shape.setPosition((y+1)*TILE_SIZE, (x+1)*TILE_SIZE);
            shape.setFillColor(sf::Color::Green);
            window.draw(shape);
         }
         break;
         }
      }
    }
    window.display();
  }

  return 0;
}
