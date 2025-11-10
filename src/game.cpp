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
}

void load_textures(std::map<std::string, sf::Texture *> &textures) {
  sf::Texture *happy_face = new sf::Texture;
  if (!happy_face->loadFromFile("./files/images/face_happy.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"face_happy", happy_face});
  sf::Texture* tile_hidden = new sf::Texture;
  if (!tile_hidden->loadFromFile("./files/images/tile_hidden.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"tile_hidden", tile_hidden});
}

void play_game() {
  Config config;
  config.read_config();
  std::vector<std::vector<Tile>> field(
      config.rows, std::vector<Tile>(config.cols, Tile{false, 0}));
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

  // Assign it to a sprite
  sf::Sprite sprite;
  sprite.setTexture(*textures.at("face_happy"));
  sprite.setPosition(config.cols / 2.0 * 32 - 32, 32 * (config.rows + 0.5));

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear(sf::Color::White);
    window.draw(sprite);
    window.display();
  }
}
