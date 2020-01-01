#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>

const int BOARD_SIZE = 40;
const float TILE_SIZE = 20.0f;

class Game {
  int board[BOARD_SIZE][BOARD_SIZE];
  int length, head_x, head_y;
  bool dead;

public:

  enum direction_t {
    Up, Down, Left, Right };
  enum content_t {
    Snake, Food, Empty, Head };

private:
  direction_t new_direction;
  direction_t direction;

  void spawn_food() {
    std::uniform_int_distribution<int> randomLocationRange(0, BOARD_SIZE-1);
    std::random_device rd;
    std::mt19937 randomNumbers(rd());
    int i = randomLocationRange( randomNumbers );
    int j = randomLocationRange( randomNumbers );
    if (          board[i][j] ==  0 )
      board[i][j] = -1;
    else
      spawn_food();
  }

public:

  void setDirection( direction_t dir ) {
    if ( dir == Left){
      if (direction != Right)
        new_direction = Left;
    }
    if ( dir == Right){
      if (direction != Left)
        new_direction = Right;
    }
    if ( dir == Up){
      if (direction != Down)
        new_direction = Up;
    }
    if ( dir == Down){
      if (direction != Up)
        new_direction = Down;
    }
  }

  void reset() {
    dead=false;
    length=2;
    for( int i=0;i<BOARD_SIZE;i++ ){
      for ( int j = 0;j<BOARD_SIZE;j++) {
        board[i][j] = 0;
      }
    }
    head_x = BOARD_SIZE / 2;
    head_y = BOARD_SIZE / 2;
    board[head_x][head_y]=length;
    direction=Left;
    new_direction=Left;
    spawn_food();
  }

  Game() {
    reset();
  }

  content_t getContent(int x, int y) {
    if (dead) {
      if (x == head_x && y == head_y) return Head;
      if (board[x][y] > 0) return Food;
      if (board[x][y] == -1 ) return Empty;
      return Snake;
    } else {
      if (x == head_x && y == head_y) return Head;
      if (board[x][y] > 0) return Snake;
      if (board[x][y] == -1 ) return Food;
      return Empty;
    }
  }

  void iterate() {
    if (dead) return;
    direction=new_direction;
    switch (direction) {
    case Left: head_x--;break;
    case Right: head_x++;break;
    case Up: head_y--;break;
    case Down: head_y++;break;
    }
    if (head_x < 0 || head_y < 0 || head_x == BOARD_SIZE || head_y == BOARD_SIZE) {
      dead = true;
      head_x = 0;
      head_y = 0;
    }else if ( board[head_x][head_y] >0 ) {
      dead = true;
      head_x = 0;
      head_y = 0;
    } else{
      if ( board[head_x][head_y] == -1 ) {
        length++;
        spawn_food();
      }
      board[head_x][head_y] = 1;
    }
    for( int i=0;i<BOARD_SIZE;i++ ) {
      for ( int j = 0;j<BOARD_SIZE;j++ ) {
        if (board[i][j] > length) {
          board[i][j]=0;
        } else if (board[i][j] > 0) {
          board[i][j]++;
        }
      }
    }
  }

};


int main()
{
  sf::RenderWindow window(sf::VideoMode((2+BOARD_SIZE) * (int)TILE_SIZE, (2+BOARD_SIZE) * (int)TILE_SIZE), "Game of Life");

  Game game;

  sf::Clock clock;

  while (window.isOpen()) {
    sf::Time elapsed = clock.getElapsedTime();
    if (elapsed.asSeconds() > 0.2f) {
      game.iterate();
      clock.restart();
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
          game.reset();
        }
        if (event.key.code == sf::Keyboard::Left){
          game.setDirection( Game::Left );
        }
        if (event.key.code == sf::Keyboard::Right){
          game.setDirection( Game::Right );
        }
        if (event.key.code == sf::Keyboard::Up){
          game.setDirection( Game::Up );
        }
        if (event.key.code == sf::Keyboard::Down){
          game.setDirection( Game::Down );
        }

      }
    }

    window.clear( sf::Color::Blue );
    for( int x=0;x<BOARD_SIZE;x++ ){
      for ( int y = 0;y<BOARD_SIZE;y++) {
        sf::RectangleShape shape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
        shape.setOrigin((x+1)*-TILE_SIZE, (y+1)*-TILE_SIZE);
        switch (game.getContent(x, y)) {
        case Game::Empty:
          shape.setFillColor(sf::Color::Black);
          break;
        case Game::Food:
          shape.setFillColor(sf::Color::Red);
          break;
        case Game::Snake:
          shape.setFillColor(sf::Color::White);
          break;
        case Game::Head:
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
