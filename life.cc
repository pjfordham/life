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
    char xCoord;
    char yCoord;
    char height;
    char width;
    const char * const* figure;
};

const char *rpentomino[] = {
   ".XX",
   "XX.",
   ".X." };

struct RPentomino : public Shape {
   RPentomino( char x , char y ) {
      figure = rpentomino;
      xCoord = x;
      yCoord = y;
      height = sizeof( rpentomino ) / sizeof( *rpentomino );
      width = strlen( rpentomino[0] );
   }
};

const char *glider[] = {
   ".X.",
   "..X",
   "XXX" };

struct Glider : public Shape {
   Glider( char x , char y ) {
      figure = glider;
      xCoord = x;
      yCoord = y;
      height = sizeof( glider ) / sizeof( *glider );
      width = strlen( glider[0] );
   }
};

const char *blinker[] = {
   "XXX" };

struct Blinker : public Shape {
   Blinker( char x , char y ) {
      figure = blinker;
      xCoord = x;
      yCoord = y;
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
   Almond( char x , char y ) {
      figure = almond;
      xCoord = x;
      yCoord = y;
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
   SpaceShip( char x , char y ) {
      figure = spaceship;
      xCoord = x;
      yCoord = y;
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
   char getState( char state , char xCoord , char yCoord , bool toggle);
   void iterate(unsigned int iterations);
private:
   char world[HEIGHT][WIDTH];
   char otherWorld[HEIGHT][WIDTH];
   bool toggle;
};

GameOfLife::GameOfLife() :
   toggle(true)
{
   for ( char i = 0; i < HEIGHT; i++ ) {
      for ( char j = 0; j < WIDTH; j++ ) {
         world[i][j] = '.';
      }
   }
}

void GameOfLife::clear() {
   toggle = true;
   for ( char i = 0; i < HEIGHT; i++ ) {
      for ( char j = 0; j < WIDTH; j++ ) {
         world[i][j] = '.';
      }
   }
}

void GameOfLife::addShape( Shape shape )
{
   for ( char i = shape.yCoord; i - shape.yCoord < shape.height; i++ ) {
      for ( char j = shape.xCoord; j - shape.xCoord < shape.width; j++ ) {
         if ( i < HEIGHT && j < WIDTH ) {
            if ( toggle ) {
               world[i][j] =
                  shape.figure[ i - shape.yCoord ][j - shape.xCoord ];
            } else {
               otherWorld[i][j] =
                  shape.figure[ i - shape.yCoord ][j - shape.xCoord ];
            }
         }
      }
   }
}

void GameOfLife::print() {
    if ( toggle ) {
        for ( char i = 0; i < HEIGHT; i++ ) {
            for ( char j = 0; j < WIDTH; j++ ) {
                std::cout << world[i][j];
            }
            std::cout << std::endl;
        }
    } else {
        for ( char i = 0; i < HEIGHT; i++ ) {
            for ( char j = 0; j < WIDTH; j++ ) {
                std::cout << otherWorld[i][j];
            }
            std::cout << std::endl;
        }
    }
    for ( char i = 0; i < WIDTH; i++ ) {
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
        for ( char i = 0; i < HEIGHT; i++ ) {
            for ( char j = 0; j < WIDTH; j++ ) {
                otherWorld[i][j] =
                    GameOfLife::getState(world[i][j] , i , j , toggle);
            }
        }
        toggle = !toggle;
    } else {
        for ( char i = 0; i < HEIGHT; i++ ) {
            for ( char j = 0; j < WIDTH; j++ ) {
                world[i][j] =
                    GameOfLife::getState(otherWorld[i][j] , i , j , toggle);
            }
        }
        toggle = !toggle;
    }
}

char GameOfLife::getState( char state, char yCoord, char xCoord, bool toggle ) {
    char neighbors = 0;
    if ( toggle ) {
        for ( char i = yCoord - 1; i <= yCoord + 1; i++ ) {
            for ( char j = xCoord - 1; j <= xCoord + 1; j++ ) {
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
        for ( char i = yCoord - 1; i <= yCoord + 1; i++ ) {
            for ( char j = xCoord - 1; j <= xCoord + 1; j++ ) {
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

  std::uniform_int_distribution<int> randomLocationRange(0, BOARD_SIZE-1);
  std::random_device rd;
  std::mt19937 randomNumbers(rd());

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
        if (event.key.code == sf::Keyboard::Space){
        }
        if (event.key.code == sf::Keyboard::A){
           gol.addShape(Almond(randomLocationRange( randomNumbers ),randomLocationRange( randomNumbers )));
        }
        if (event.key.code == sf::Keyboard::S){
           gol.addShape(SpaceShip(randomLocationRange( randomNumbers ),randomLocationRange( randomNumbers )));
        }
        if (event.key.code == sf::Keyboard::G){
           gol.addShape(Glider(randomLocationRange( randomNumbers ),randomLocationRange( randomNumbers )));
        }
        if (event.key.code == sf::Keyboard::X){
           gol.addShape(RPentomino(randomLocationRange( randomNumbers ),randomLocationRange( randomNumbers )));
        }
        if (event.key.code == sf::Keyboard::B){
           gol.addShape(Blinker(randomLocationRange( randomNumbers ),randomLocationRange( randomNumbers )));
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
        if (event.key.code == sf::Keyboard::R){
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

    window.clear( sf::Color::Blue );
    for( int x=0;x<BOARD_SIZE;x++ ){
      for ( int y = 0;y<BOARD_SIZE;y++) {
        sf::RectangleShape shape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
        shape.setOrigin((y+1)*-TILE_SIZE, (x+1)*-TILE_SIZE);
        switch (gol.getContent(x, y)) {
        case Empty:
          shape.setFillColor(sf::Color::Black);
          break;
        case Food:
          shape.setFillColor(sf::Color::Red);
          break;
        case Snake:
          shape.setFillColor(sf::Color::White);
          break;
        case Head:
          shape.setFillColor(sf::Color::Green);
          break;
        }
        window.draw(shape);
      }
    }

    window.display();
  }

  return 0;
}
