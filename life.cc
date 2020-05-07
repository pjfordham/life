#include <tuple>
#include <string.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>

#include <iostream>
#include <utility>

#include "support.hh"

const int BOARD_SIZE = 100;
const float TILE_SIZE = 10.0f;

const int HEIGHT=BOARD_SIZE;
const int WIDTH=BOARD_SIZE;

struct Shape {
public:
   int height;
   int width;
   const char * const* figure;
   const char *name;
};

#define SubShape( NAME, ... )                             \
   struct NAME : public Shape {                           \
      NAME() {                                            \
         static const char *shape[] = { __VA_ARGS__ };    \
         static const char *Name = #NAME;                 \
         name = Name;                                     \
         figure = shape;                                  \
         height = sizeof( shape ) / sizeof( *shape );     \
         width = strlen( shape[0] );                      \
      }                                                   \
   };

SubShape( Crab,                                 \
          "....X.....X....",                    \
          "....X.....X....",                    \
          "....XX...XX....",                    \
          "...............",                    \
          "XXX..XX.XX..XXX",                    \
          "..X.X.X.X.X.X..",                    \
          "....XX...XX....",                    \
          "...............",                    \
          "....XX...XX....",                    \
          "..X.X.X.X.X.X..",                    \
          "XXX..XX.XX..XXX",                    \
          "...............",                    \
          "....XX...XX....",                    \
          "....X.....X....",                    \
          "....X.....X...." )

SubShape(RPentomino,                            \
         ".XX",                                 \
         "XX.",                                 \
         ".X." )

SubShape(Glider,                                \
         ".X.",                                 \
         "..X",                                 \
         "XXX" )

SubShape(Blinker, \
         "XXX" )

SubShape(Almond,                                \
         ".X.",                                 \
         "X.X",                                 \
         "X.X",                                 \
         ".X." )

SubShape(SpaceShip,                             \
         "X...X.",                              \
         ".....X",                              \
         "X....X",                              \
         ".XXXXX" )

class GameOfLife {
public:
   enum state_t {
      LIVE = 10,
      DEAD = 0,
   };

   GameOfLife();
   void addShape( Shape shape, int x = -1 , int y = -1);
   void click( int i, int j );

   void print();
   void update();
   void clear();
   state_t getContent( int i, int j);
   state_t getState( state_t state , int x , int y );
   void iterate(unsigned int iterations);
private:
   Array2D<state_t> world;
   Array2D<state_t> otherWorld;

   std::random_device rd;
   std::mt19937 randomNumbers;
};

GameOfLife::GameOfLife() : world(HEIGHT,WIDTH), otherWorld(HEIGHT,WIDTH), randomNumbers(rd()) {
   clear();
}

void GameOfLife::clear() {
   for ( int i = 0; i < HEIGHT; i++ ) {
      for ( int j = 0; j < WIDTH; j++ ) {
         world[i][j] = DEAD;
      }
   }
}

void GameOfLife::click( int j, int i )
{
   if ( world[i][j] == LIVE ){
      world[i][j] = DEAD;
   } else {
      world[i][j] = LIVE;
   }
}

void GameOfLife::addShape( Shape shape, int x, int y )
{
   std::uniform_int_distribution<int> randomXLocationRange(0, BOARD_SIZE-shape.width);
   std::uniform_int_distribution<int> randomYLocationRange(0, BOARD_SIZE-shape.height);

   if ( x == -1 )
      x = randomXLocationRange( randomNumbers );
   if ( y == -1 )
      y = randomYLocationRange( randomNumbers );

   for ( int i = y; i - y < shape.height; i++ ) {
      for ( int j = x; j - x < shape.width; j++ ) {
         if ( i < HEIGHT && j < WIDTH ) {
            world[i][j] =
               shape.figure[ i - y ][j - x ] == 'X' ? LIVE : DEAD;
         }
      }
   }
}

void GameOfLife::print() {
   for ( int i = 0; i < HEIGHT; i++ ) {
      for ( int j = 0; j < WIDTH; j++ ) {
         std::cout << world[i][j];
      }
      std::cout << std::endl;
   }
   for ( int i = 0; i < WIDTH; i++ ) {
      std::cout << '=';
   }
   std::cout << std::endl;
}

GameOfLife::state_t GameOfLife::getContent(int i, int j) {
   return world[i][j];
}

void GameOfLife::update() {
   for ( int i = 0; i < HEIGHT; i++ ) {
      for ( int j = 0; j < WIDTH; j++ ) {
         otherWorld[i][j] =
            GameOfLife::getState(world[i][j] , i , j);
      }
   }
   std::swap(world, otherWorld);
}

GameOfLife::state_t GameOfLife::getState( state_t state, int y, int x ) {
    int neighbors = 0;
    for ( int i = y - 1; i <= y + 1; i++ ) {
       for ( int j = x - 1; j <= x + 1; j++ ) {
          if ( i == y && j == x ) {
             continue;
          }
          if ( i > -1 && i < HEIGHT && j > -1 && j < WIDTH ) {
             if ( world[i][j] == LIVE ) {
                neighbors++;
             }
          }
       }
    }
    if (state == LIVE) {
       return ( neighbors > 1 && neighbors < 4 ) ? LIVE : DEAD;
    }
    else {
       return ( neighbors == 3 ) ? LIVE : DEAD;
    }
}

void GameOfLife::iterate( unsigned int iterations ) {
    for ( int i = 0; i < iterations; i++ ) {
        update();
    }
}


int main()
{

   GameOfLife gol;

   Shape shapes[] = { Almond(), Glider(), Crab(), RPentomino(), SpaceShip(), Blinker() };
   int shapeIndex = 0;

   sf::Font font;
   if (!font.loadFromFile("Instruction.ttf") ) {
      std::cerr << "Font error." << std::endl;
      exit( -1 );
   }

   sf::RenderWindow window(sf::VideoMode((2+BOARD_SIZE) * (int)TILE_SIZE, (2+BOARD_SIZE) * (int)TILE_SIZE), "Game of Life");

   sf::Clock clock;
   sf::Clock messageClock;

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
         if (event.type == sf::Event::Closed) {
            window.close();
         } else if (event.type == sf::Event::MouseWheelScrolled) {
            if(event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel ) {
               messageClock.restart();
               shapeIndex+=event.mouseWheelScroll.delta;
               if (shapeIndex < 0)
                  shapeIndex = ( sizeof( shapes ) / sizeof (shapes[0] ) ) - 1;
               if (shapeIndex >= ( sizeof( shapes ) / sizeof (shapes[0] ) ))
                  shapeIndex = 0;
            }
         } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
               gol.click( ((int)event.mouseButton.x / (int)TILE_SIZE ) - 1,
                          ((int)event.mouseButton.y / (int)TILE_SIZE ) - 1 );
            } else if (event.mouseButton.button == sf::Mouse::Right) {
               gol.addShape( shapes[ shapeIndex],
                             ((int)event.mouseButton.x / (int)TILE_SIZE ) - 1,
                             ((int)event.mouseButton.y / (int)TILE_SIZE ) - 1 );
            }
         }
         else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape){
               return 0;
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
               running = !running;
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
            case GameOfLife::DEAD:
               // Do nothing
               break;
            case GameOfLife::LIVE:
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
      sf::Time elapsed = messageClock.getElapsedTime();
      if (elapsed.asSeconds() < 2.0f) {
         sf::Text text;
         text.setFont(font);
         text.setString(shapes[shapeIndex].name);
         text.setCharacterSize(4*TILE_SIZE); // in pixels, not points!
         text.setPosition(4*TILE_SIZE, 4*TILE_SIZE);
         text.setFillColor(sf::Color::Red);
         window.draw(text);
      }

      window.display();
   }

   return 0;
}
