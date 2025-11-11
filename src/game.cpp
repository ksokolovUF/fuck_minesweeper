#include "game.hpp"
#include "config.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <random>
#include <vector>

void place_mines(std::vector<std::vector<Tile>> &field,
                 const unsigned int mines) {
  std::random_device random_device;
  std::mt19937 engine{random_device()};
  unsigned int count = 0;
  while (count < 50) {
    std::uniform_int_distribution<int> dist_rows(0, field.size() - 1);
    std::uniform_int_distribution<int> dist_cols(0, field.at(0).size() - 1);
    Tile &tile = field.at(dist_rows(engine)).at(dist_cols(engine));
    if (tile.has_bomb == false) {
      tile.has_bomb = true;
      count++;
    }
  }
  /*
  for (int i = 0; i < field.size(); i++) {
    for (int j = 0; j < field.at(0).size(); j++) {
      if (field.at(i).at(j).has_bomb == true) {
        std::cout << "X";
      } else {
        std::cout << "O";
      }
    }
    std::cout << std::endl;
  }
  */
}

void load_textures(std::map<std::string, sf::Texture *> &textures) {
  sf::Texture *happy_face = new sf::Texture;
  if (!happy_face->loadFromFile("./files/images/face_happy.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"face_happy", happy_face});
  sf::Texture *tile_hidden = new sf::Texture;
  if (!tile_hidden->loadFromFile("./files/images/tile_hidden.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"tile_hidden", tile_hidden});
  sf::Texture *mine = new sf::Texture;
  if (!mine->loadFromFile("./files/images/mine.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"mine", mine});
  sf::Texture *debug = new sf::Texture;
  if (!debug->loadFromFile("./files/images/debug.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"debug", debug});
  sf::Texture *play = new sf::Texture;
  if (!play->loadFromFile("./files/images/play.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"play", play});
  sf::Texture *pause = new sf::Texture;
  if (!pause->loadFromFile("./files/images/pause.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"pause", pause});
  sf::Texture *digits = new sf::Texture;
  if (!digits->loadFromFile("./files/images/digits.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"digits", digits});
  sf::Texture *leaderboard = new sf::Texture;
  if (!leaderboard->loadFromFile("./files/images/leaderboard.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"leaderboard", leaderboard});
}

void load_hidden_mines(const std::vector<std::vector<Tile>> &field,
                       const std::map<std::string, sf::Texture *> &textures,
                       std::vector<sf::Sprite> &mine_sprites) {
  unsigned int mine_index = 0;
  for (int i = 0; i < field.size(); i++) {
    for (int j = 0; j < field.at(0).size(); j++) {
      if (field.at(i).at(j).has_bomb == true) {
        mine_sprites.at(mine_index).setTexture(*textures.at("mine"));
        mine_sprites.at(mine_index)
            .setPosition(field.at(i).at(j).x * 32, field.at(i).at(j).y * 32);
        mine_index++;
      }
    }
  }
}

void play_game() {
  Config config;
  config.read_config();
  bool debug_on = false;
  std::vector<std::vector<Tile>> field(
      config.rows, std::vector<Tile>(config.cols, Tile{false, 0}));
  for (int i = 0; i < field.size(); i++) {
    for (int j = 0; j < field.at(0).size(); j++) {
      field.at(i).at(j).x = j;
      field.at(i).at(j).y = i;
    }
  }
  place_mines(field, config.mines);

  const unsigned int window_height = config.rows * 32 + 100;
  const unsigned int window_width = config.cols * 32;

  sf::RenderWindow window(sf::VideoMode(window_width, window_height),
                          "SFML window", sf::Style::Close);
  sf::Font font;
  if (!font.loadFromFile("./files/font.ttf")) {
    throw "fucked up font path";
  }

  std::map<std::string, sf::Texture *> textures;
  load_textures(textures);

  std::vector<sf::Sprite> debug_mine_sprites(config.mines);
  load_hidden_mines(field, textures, debug_mine_sprites);

  sf::Sprite face_happy_sprite;
  face_happy_sprite.setTexture(*textures.at("face_happy"));
  face_happy_sprite.setPosition(config.cols / 2.0 * 32 - 32,
                                32 * (config.rows + 0.5));
  sf::Sprite debug_sprite;
  debug_sprite.setTexture(*textures.at("debug"));
  debug_sprite.setPosition(config.cols * 32 - 304, 32 * (config.rows + 0.5));
  sf::Sprite play_pause_sprite;
  play_pause_sprite.setTexture(*textures.at("pause"));
  play_pause_sprite.setPosition(config.cols * 32 - 240,
                                32 * (config.rows + 0.5));
  sf::Sprite leaderboard_sprite;
  leaderboard_sprite.setTexture(*textures.at("leaderboard"));
  leaderboard_sprite.setPosition(config.cols * 32 - 177,
                                 32 * (config.rows + 0.5));

  std::vector<std::vector<sf::Sprite>> tile_sprites(
      field.size(), std::vector<sf::Sprite>(field.at(0).size()));
  for (int i = 0; i < field.size(); i++) {
    for (int j = 0; j < field.at(0).size(); j++) {
      tile_sprites.at(i).at(j).setTexture(*textures.at("tile_hidden"));
      tile_sprites.at(i).at(j).setPosition(32 * j, 32 * i);
    }
  }

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f pixel_clicked =
            window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (debug_sprite.getGlobalBounds().contains(pixel_clicked)) {
          if (debug_on == false) {
            debug_on = true;
          } else if (debug_on == true) {
            debug_on = false;
          }
        }
      }
    }

    window.clear(sf::Color::White);
    for (int i = 0; i < field.size(); i++) {
      for (int j = 0; j < field.at(0).size(); j++) {
        window.draw(tile_sprites.at(i).at(j));
      }
    }
    if (debug_on == true) {
      for (int i = 0; i < config.mines; i++) {
        window.draw(debug_mine_sprites.at(i));
      }
    }
    window.draw(face_happy_sprite);
    window.draw(debug_sprite);
    window.draw(play_pause_sprite);
    window.draw(leaderboard_sprite);
    window.display();
  }
}
