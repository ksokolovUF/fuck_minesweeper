#include "game.hpp"
#include "config.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include <map>
#include <random>
#include <vector>

bool is_on_field(const int current_row, const int current_col,
                 const size_t board_rows, const size_t board_cols) {
  if (current_row < board_rows && current_col < board_cols &&
      current_col >= 0 && current_row >= 0) {
    return true;
  } else {
    return false;
  }
}

void clear_empty_tiles(std::vector<std::vector<Tile>> &field,
                       const std::map<std::string, sf::Texture *> &textures,
                       std::vector<std::vector<sf::Sprite>> &tile_sprites,
                       const size_t row, const size_t col) {
  std::cout << row << " " << col << std::endl;
  tile_sprites.at(row).at(col).setTexture(*textures.at("tile_revealed"));
  field.at(row).at(col).hidden = false;
  for (std::pair<int, int> tile : Config::NEIGHBOR_OFFSETS) {
    const int current_row = static_cast<int>(row) + tile.first;
    const int current_col = static_cast<int>(col) + tile.second;
    if (is_on_field(current_row, current_col, field.size(),
                    field.at(0).size()) &&
        field.at(current_row).at(current_col).hidden == true &&
        field.at(current_row).at(current_col).mines_nearby == 0) {
      clear_empty_tiles(field, textures, tile_sprites, current_row,
                        current_col);
    }
  }
  for (std::pair<int, int> tile : Config::NEIGHBOR_OFFSETS) {
    const int current_row = static_cast<int>(row) + tile.first;
    const int current_col = static_cast<int>(col) + tile.second;
    if (is_on_field(current_row, current_col, field.size(),
                    field.at(0).size()) &&
        field.at(current_row).at(current_col).hidden == true) {
      tile_sprites.at(current_row)
          .at(current_col)
          .setTexture(*textures.at("tile_revealed"));
      field.at(current_row).at(current_col).hidden = false;
    }
  }
}

int calculate_neighboring_mines(const std::vector<std::vector<Tile>> &field,
                                const size_t row, const size_t col) {
  if (!is_on_field(row, col, field.size(), field.at(0).size())) {
    return 0;
  }
  int num_alive = 0;
  for (std::pair<int, int> tile : Config::NEIGHBOR_OFFSETS) {
    const int current_row = static_cast<int>(row) + tile.first;
    const int current_col = static_cast<int>(col) + tile.second;

    if (is_on_field(current_row, current_col, field.size(),
                    field.at(0).size()) &&
        field.at(current_row).at(current_col).has_mine == true) {
      num_alive++;
    }
  }
  return num_alive;
}

void place_mines(std::vector<std::vector<Tile>> &field,
                 const unsigned int mines) {
  std::random_device random_device;
  std::mt19937 engine{random_device()};
  unsigned int count = 0;
  while (count < 50) {
    std::uniform_int_distribution<int> dist_rows(0, field.size() - 1);
    std::uniform_int_distribution<int> dist_cols(0, field.at(0).size() - 1);
    Tile &tile = field.at(dist_rows(engine)).at(dist_cols(engine));
    if (tile.has_mine == false) {
      tile.has_mine = true;
      count++;
    }
  }
  for (int i = 0; i < field.size(); i++) {
    for (int j = 0; j < field.at(0).size(); j++) {
      field.at(i).at(j).mines_nearby = calculate_neighboring_mines(field, i, j);
    }
  }
  for (int i = 0; i < field.size(); i++) {
    for (int j = 0; j < field.at(0).size(); j++) {
      if (field.at(i).at(j).has_mine == true) {
        std::cout << "X";
      } else {
        std::cout << field.at(i).at(j).mines_nearby;
      }
    }
    std::cout << std::endl;
  }
}

void load_textures(std::map<std::string, sf::Texture *> &textures) {
  sf::Texture *face_happy = new sf::Texture;
  if (!face_happy->loadFromFile("./files/images/face_happy.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"face_happy", face_happy});
  sf::Texture *face_lose = new sf::Texture;
  if (!face_lose->loadFromFile("./files/images/face_lose.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"face_lose", face_lose});
  sf::Texture *face_win = new sf::Texture;
  if (!face_win->loadFromFile("./files/images/face_win.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"face_win", face_win});
  sf::Texture *tile_hidden = new sf::Texture;
  if (!tile_hidden->loadFromFile("./files/images/tile_hidden.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"tile_hidden", tile_hidden});
  sf::Texture *tile_revealed = new sf::Texture;
  if (!tile_revealed->loadFromFile("./files/images/tile_revealed.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"tile_revealed", tile_revealed});
  sf::Texture *mine = new sf::Texture;
  if (!mine->loadFromFile("./files/images/mine.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"mine", mine});
  sf::Texture *flag = new sf::Texture;
  if (!flag->loadFromFile("./files/images/flag.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"flag", flag});
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
  sf::Texture *number_1 = new sf::Texture;
  if (!number_1->loadFromFile("./files/images/number_1.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"number_1", number_1});
  sf::Texture *number_2 = new sf::Texture;
  if (!number_2->loadFromFile("./files/images/number_2.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"number_2", number_2});
  sf::Texture *number_3 = new sf::Texture;
  if (!number_3->loadFromFile("./files/images/number_3.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"number_3", number_3});
  sf::Texture *number_4 = new sf::Texture;
  if (!number_4->loadFromFile("./files/images/number_4.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"number_4", number_4});
  sf::Texture *number_5 = new sf::Texture;
  if (!number_5->loadFromFile("./files/images/number_5.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"number_5", number_5});
  sf::Texture *number_6 = new sf::Texture;
  if (!number_6->loadFromFile("./files/images/number_6.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"number_6", number_6});
  sf::Texture *number_7 = new sf::Texture;
  if (!number_7->loadFromFile("./files/images/number_7.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"number_7", number_7});
  sf::Texture *number_8 = new sf::Texture;
  if (!number_8->loadFromFile("./files/images/number_8.png")) {
    throw "fucked up texture path";
  }
  textures.insert({"number_8", number_8});
}

void load_hidden_mines(const std::vector<std::vector<Tile>> &field,
                       const std::map<std::string, sf::Texture *> &textures,
                       std::vector<sf::Sprite> &mine_sprites) {
  unsigned int mine_index = 0;
  for (int i = 0; i < field.size(); i++) {
    for (int j = 0; j < field.at(0).size(); j++) {
      if (field.at(i).at(j).has_mine == true) {
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
  bool paused = false;
  bool raspidorasilo = false;
  std::vector<std::vector<Tile>> field(config.rows,
                                       std::vector<Tile>(config.cols, Tile{}));
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
      if (field.at(i).at(j).hidden == true) {
        tile_sprites.at(i).at(j).setTexture(*textures.at("tile_hidden"));
      } else {
        tile_sprites.at(i).at(j).setTexture(*textures.at("tile_revealed"));
      }
      tile_sprites.at(i).at(j).setPosition(32 * j, 32 * i);
    }
  }
  std::vector<std::vector<sf::Sprite>> mine_or_digit_sprites(
      field.size(), std::vector<sf::Sprite>(field.at(0).size()));
  for (int i = 0; i < field.size(); i++) {
    for (int j = 0; j < field.at(0).size(); j++) {
      if (field.at(i).at(j).has_mine == true) {
        mine_or_digit_sprites.at(i).at(j).setTexture(*textures.at("mine"));
        raspidorasilo = true;
      } else {
        switch (field.at(i).at(j).mines_nearby) {
        case 1:
          mine_or_digit_sprites.at(i).at(j).setTexture(
              *textures.at("number_1"));
          break;
        case 2:
          mine_or_digit_sprites.at(i).at(j).setTexture(
              *textures.at("number_2"));
          break;
        case 3:
          mine_or_digit_sprites.at(i).at(j).setTexture(
              *textures.at("number_3"));
          break;
        case 4:
          mine_or_digit_sprites.at(i).at(j).setTexture(
              *textures.at("number_4"));
          break;
        case 5:
          mine_or_digit_sprites.at(i).at(j).setTexture(
              *textures.at("number_5"));
          break;
        case 6:
          mine_or_digit_sprites.at(i).at(j).setTexture(
              *textures.at("number_6"));
          break;
        case 7:
          mine_or_digit_sprites.at(i).at(j).setTexture(
              *textures.at("number_7"));
          break;
        case 8:
          mine_or_digit_sprites.at(i).at(j).setTexture(
              *textures.at("number_8"));
          break;
        default:
          break;
        }
      }
      mine_or_digit_sprites.at(i).at(j).setPosition(32 * j, 32 * i);
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

        // clicked debug button
        if (debug_sprite.getGlobalBounds().contains(pixel_clicked)) {
          if (debug_on == false) {
            debug_on = true;
          } else if (debug_on == true) {
            debug_on = false;
          }
        }

        // clicked debug button
        if (play_pause_sprite.getGlobalBounds().contains(pixel_clicked)) {
          if (paused == false) {
            paused = true;
            play_pause_sprite.setTexture(*textures.at("play"));
          } else if (paused == true) {
            paused = false;
            play_pause_sprite.setTexture(*textures.at("pause"));
          }
        }

        // clicked tile
        for (int i = 0; i < tile_sprites.size(); i++) {
          for (int j = 0; j < tile_sprites.at(0).size(); j++) {
            if (tile_sprites.at(i).at(j).getGlobalBounds().contains(
                    pixel_clicked)) {
              tile_sprites.at(i).at(j).setTexture(
                  *textures.at("tile_revealed"));
              field.at(i).at(j).hidden = false;
              if (field.at(i).at(j).has_mine == false &&
                  field.at(i).at(j).mines_nearby == 0) {
                clear_empty_tiles(field, textures, tile_sprites, i, j);
              }
            }
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
    for (int i = 0; i < field.size(); i++) {
      for (int j = 0; j < field.at(0).size(); j++) {
        if (field.at(i).at(j).hidden == false) {
          window.draw(mine_or_digit_sprites.at(i).at(j));
        }
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
