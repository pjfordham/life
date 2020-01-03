#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>

#include <iostream>

const int BOARD_SIZE = 40;
const float TILE_SIZE = 20.0f;

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
    char **figure;
};

struct Glider : public Shape {
    static const char GLIDER_SIZE = 3;
    Glider( char x , char y );
    ~Glider();
};

struct Blinker : public Shape {
    static const char BLINKER_HEIGHT = 3;
    static const char BLINKER_WIDTH = 1;
    Blinker( char x , char y );
    ~Blinker();
};

struct Almond : public Shape {
    static const char ALMOND_HEIGHT = 4;
    static const char ALMOND_WIDTH = 3;
    Almond( char x , char y );
    ~Almond();
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

Glider::Glider( char x , char y ) {
    xCoord = x;
    yCoord = y;
    height = GLIDER_SIZE;
    width = GLIDER_SIZE;
    figure = new char*[GLIDER_SIZE];
    for ( char i = 0; i < GLIDER_SIZE; i++ ) {
        figure[i] = new char[GLIDER_SIZE];
    }
    for ( char i = 0; i < GLIDER_SIZE; i++ ) {
        for ( char j = 0; j < GLIDER_SIZE; j++ ) {
            figure[i][j] = '.';
        }
    }
    figure[0][1] = 'X';
    figure[1][2] = 'X';
    figure[2][0] = 'X';
    figure[2][1] = 'X';
    figure[2][2] = 'X';
}

Glider::~Glider() {
    for ( char i = 0; i < GLIDER_SIZE; i++ ) {
        delete[] figure[i];
    }
    delete[] figure;
}

Blinker::Blinker( char x , char y ) {
    xCoord = x;
    yCoord = y;
    height = BLINKER_HEIGHT;
    width = BLINKER_WIDTH;
    figure = new char*[BLINKER_HEIGHT];
    for ( char i = 0; i < BLINKER_HEIGHT; i++ ) {
        figure[i] = new char[BLINKER_WIDTH];
    }
    for ( char i = 0; i < BLINKER_HEIGHT; i++ ) {
        for ( char j = 0; j < BLINKER_WIDTH; j++ ) {
            figure[i][j] = 'X';
        }
    }
}

Blinker::~Blinker() {
    for ( char i = 0; i < BLINKER_HEIGHT; i++ ) {
        delete[] figure[i];
    }
    delete[] figure;
}

Almond::Almond( char x , char y ) {
    xCoord = x;
    yCoord = y;
    height = ALMOND_HEIGHT;
    width = ALMOND_WIDTH;
    figure = new char*[ALMOND_HEIGHT];
    for ( char i = 0; i < ALMOND_HEIGHT; i++ ) {
        figure[i] = new char[ALMOND_WIDTH];
    }
    for ( char i = 0; i < ALMOND_HEIGHT; i++ ) {
        for ( char j = 0; j < ALMOND_WIDTH; j++ ) {
            figure[i][j] = '.';
        }
    }
    figure[0][1] = 'X';
    figure[1][0] = 'X';
    figure[1][2] = 'X';
    figure[2][0] = 'X';
    figure[2][2] = 'X';
    figure[3][1] = 'X';
}

Almond::~Almond() {
    for ( char i = 0; i < ALMOND_HEIGHT; i++ ) {
        delete[] figure[i];
    }
    delete[] figure;
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
        if (event.key.code == sf::Keyboard::G){
           gol.addShape(Glider(randomLocationRange( randomNumbers ),randomLocationRange( randomNumbers )));
        }
        if (event.key.code == sf::Keyboard::B){
           gol.addShape(Blinker(randomLocationRange( randomNumbers ),randomLocationRange( randomNumbers )));
        }
        if (event.key.code == sf::Keyboard::I){
           gol.iterate(1);
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
        shape.setOrigin((x+1)*-TILE_SIZE, (y+1)*-TILE_SIZE);
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
