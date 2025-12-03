#include "game.hpp"
#include "config.hpp"
#include "leaderboard.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <assert.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <tuple>
#include <vector>

void end_game() {}

void record_top(const int minutes, const int seconds, const Config &config) {
  std::string minutes_str;
  std::string seconds_str;
  if (minutes == 0) {
    minutes_str = "00";
  } else if (minutes / 10 == 0) {
    minutes_str = "0" + std::to_string(minutes);
  }
  if (seconds == 0) {
    seconds_str = "00";
  } else if (seconds / 10 == 0) {
    seconds_str = "0" + std::to_string(seconds);
  }
  std::fstream ifile("files/leaderboard.txt");
  std::string top_str;
  bool player_time_recorded = false;
  if (ifile) {
    std::string line;
    int index = 1;
    int current_mins;
    int current_secs;
    while (std::getline(ifile, line) && index <= 5) {
      current_mins = std::stoi(line.substr(0, 2));
      current_secs = std::stoi(line.substr(3, 2));
      int current_time = current_mins * 60 + current_secs;
      int player_time = minutes * 60 + seconds;
      if (player_time <= current_time && !player_time_recorded) {
        player_time_recorded = true;
        std::string new_line =
            minutes_str + ":" + seconds_str + ", " + Config::name;
        top_str += new_line;
        top_str += '\n';
        index++;
        top_str += line;
        top_str += '\n';
        index++;
      } else {
        top_str += line;
        top_str += '\n';
        index++;
      }
    }
  }
  ifile.close();
  std::ofstream ofile("files/leaderboard.txt", std::ios::trunc);
  ofile << top_str;
  ofile.close();
}

void match_digit(const unsigned int num, sf::Sprite &sprite) {
  sf::Rect flags_digit_0_rect(0, 0, 21, 32);
  sf::Rect flags_digit_1_rect(21, 0, 21, 32);
  sf::Rect flags_digit_2_rect(2 * 21, 0, 21, 32);
  sf::Rect flags_digit_3_rect(3 * 21, 0, 21, 32);
  sf::Rect flags_digit_4_rect(4 * 21, 0, 21, 32);
  sf::Rect flags_digit_5_rect(5 * 21, 0, 21, 32);
  sf::Rect flags_digit_6_rect(6 * 21, 0, 21, 32);
  sf::Rect flags_digit_7_rect(7 * 21, 0, 21, 32);
  sf::Rect flags_digit_8_rect(8 * 21, 0, 21, 32);
  sf::Rect flags_digit_9_rect(9 * 21, 0, 21, 32);
  sf::Rect flags_digit_negative_sign_rect(10 * 21, 0, 21, 32);
  switch (num) {
  case 0:
    sprite.setTextureRect(flags_digit_0_rect);
    break;
  case 1:
    sprite.setTextureRect(flags_digit_1_rect);
    break;
  case 2:
    sprite.setTextureRect(flags_digit_2_rect);
    break;
  case 3:
    sprite.setTextureRect(flags_digit_3_rect);
    break;
  case 4:
    sprite.setTextureRect(flags_digit_4_rect);
    break;
  case 5:
    sprite.setTextureRect(flags_digit_5_rect);
    break;
  case 6:
    sprite.setTextureRect(flags_digit_6_rect);
    break;
  case 7:
    sprite.setTextureRect(flags_digit_7_rect);
    break;
  case 8:
    sprite.setTextureRect(flags_digit_8_rect);
    break;
  case 9:
    sprite.setTextureRect(flags_digit_9_rect);
    break;
  default:
    sprite.setTextureRect(flags_digit_9_rect);
    break;
  }
}

std::tuple<sf::Sprite, sf::Sprite, sf::Sprite>
set_flag_digits(const std::map<std::string, sf::Texture *> &textures,
                const unsigned int rows, const int num) {
  std::string num_str = std::to_string(num);
  assert(num_str.size() <= 3);

  sf::Sprite flags_first_digit_sprite;
  sf::Sprite flags_second_digit_sprite;
  sf::Sprite flags_third_digit_sprite;
  flags_first_digit_sprite.setTexture(*textures.at("digits"));
  flags_second_digit_sprite.setTexture(*textures.at("digits"));
  flags_third_digit_sprite.setTexture(*textures.at("digits"));

  sf::Rect flags_digit_0_rect(0, 0, 21, 32);
  sf::Rect flags_digit_negative_sign_rect(10 * 21, 0, 21, 32);
  if (num_str.size() == 3) {
    if (num < 0) {
      flags_first_digit_sprite.setTextureRect(flags_digit_negative_sign_rect);
    } else {
      match_digit(std::stoi(num_str.substr(0, 1)), flags_first_digit_sprite);
    }
    match_digit(std::stoi(num_str.substr(1, 1)), flags_second_digit_sprite);
    match_digit(std::stoi(num_str.substr(2, 1)), flags_third_digit_sprite);
  } else if (num_str.size() == 2) {
    if (num < 0) {
      flags_second_digit_sprite.setTextureRect(flags_digit_negative_sign_rect);
    } else {
      match_digit(std::stoi(num_str.substr(0, 1)), flags_second_digit_sprite);
    }
    flags_first_digit_sprite.setTextureRect(flags_digit_0_rect);
    match_digit(std::stoi(num_str.substr(1, 1)), flags_third_digit_sprite);
  } else if (num_str.size() == 1) {
    flags_first_digit_sprite.setTextureRect(flags_digit_0_rect);
    flags_second_digit_sprite.setTextureRect(flags_digit_0_rect);
    match_digit(std::stoi(num_str.substr(0, 1)), flags_third_digit_sprite);
  }

  flags_first_digit_sprite.setPosition(33, 32 * (rows + 0.5));
  flags_second_digit_sprite.setPosition(33 + 21, 32 * (rows + 0.5));
  flags_third_digit_sprite.setPosition(33 + 2 * 21, 32 * (rows + 0.5));
  return std::make_tuple(flags_first_digit_sprite, flags_second_digit_sprite,
                         flags_third_digit_sprite);
}

std::tuple<sf::Sprite, sf::Sprite, sf::Sprite, sf::Sprite>
set_timer_digits(const std::map<std::string, sf::Texture *> &textures,
                 const unsigned int rows, const unsigned int cols,
                 const int minutes, const int seconds) {
  std::string min_str = std::to_string(minutes);
  assert(min_str.size() <= 2);
  std::string sec_str = std::to_string(seconds);
  assert(sec_str.size() <= 2);

  sf::Sprite min_first_digit_sprite;
  sf::Sprite min_second_digit_sprite;
  sf::Sprite sec_first_digit_sprite;
  sf::Sprite sec_second_digit_sprite;
  min_first_digit_sprite.setTexture(*textures.at("digits"));
  min_second_digit_sprite.setTexture(*textures.at("digits"));
  sec_first_digit_sprite.setTexture(*textures.at("digits"));
  sec_second_digit_sprite.setTexture(*textures.at("digits"));

  sf::Rect digit_0_rect(0, 0, 21, 32);
  sf::Rect digit_negative_sign_rect(10 * 21, 0, 21, 32);
  if (min_str.size() == 2) {
    match_digit(std::stoi(min_str.substr(0, 1)), min_first_digit_sprite);
    match_digit(std::stoi(min_str.substr(1, 1)), min_second_digit_sprite);
  } else if (min_str.size() == 1) {
    min_first_digit_sprite.setTextureRect(digit_0_rect);
    match_digit(std::stoi(min_str.substr(0, 1)), min_second_digit_sprite);
  }
  if (sec_str.size() == 2) {
    match_digit(std::stoi(sec_str.substr(0, 1)), sec_first_digit_sprite);
    match_digit(std::stoi(sec_str.substr(1, 1)), sec_second_digit_sprite);
  } else if (sec_str.size() == 1) {
    sec_first_digit_sprite.setTextureRect(digit_0_rect);
    match_digit(std::stoi(sec_str.substr(0, 1)), sec_second_digit_sprite);
  }

  min_first_digit_sprite.setPosition(cols * 32 - 97, 32 * (rows + 0.5) + 16);
  min_second_digit_sprite.setPosition(cols * 32 - 97 + 21,
                                      32 * (rows + 0.5) + 16);
  sec_first_digit_sprite.setPosition(cols * 32 - 54, 32 * (rows + 0.5) + 16);
  sec_second_digit_sprite.setPosition(cols * 32 - 54 + 21,
                                      32 * (rows + 0.5) + 16);
  return std::make_tuple(min_first_digit_sprite, min_second_digit_sprite,
                         sec_first_digit_sprite, sec_second_digit_sprite);
}

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
  if (field.at(row).at(col).flagged == false) {
    tile_sprites.at(row).at(col).setTexture(*textures.at("tile_revealed"));
    field.at(row).at(col).hidden = false;
  }
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
        field.at(current_row).at(current_col).hidden == true &&
        field.at(current_row).at(current_col).flagged == false) {
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
  int mines_nearby = 0;
  for (std::pair<int, int> tile : Config::NEIGHBOR_OFFSETS) {
    const int current_row = static_cast<int>(row) + tile.first;
    const int current_col = static_cast<int>(col) + tile.second;

    if (is_on_field(current_row, current_col, field.size(),
                    field.at(0).size()) &&
        field.at(current_row).at(current_col).has_mine == true) {
      mines_nearby++;
    }
  }
  return mines_nearby;
}

void place_mines(std::vector<std::vector<Tile>> &field,
                 const unsigned int mines) {
  std::random_device random_device;
  std::mt19937 engine{random_device()};
  unsigned int count = 0;
  while (count < mines) {
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

bool play_game() {
  Config config;
  bool debug_on = false;
  bool raspidorasilo = false;
  int flags_to_place = config.mines;
  int tiles_revealed = 0;
  bool won = false;
  bool ran_victory_stuff = false;
  auto start = std::chrono::steady_clock::now();
  auto stop = std::chrono::steady_clock::now();
  std::chrono::duration<double> seconds_before_pause =
      stop - start; // idk how to write 0
  std::chrono::duration<double> seconds_after_pause =
      stop - start; // idk how to write 0
  std::vector<std::vector<Tile>> field(config.rows,
                                       std::vector<Tile>(config.cols, Tile{}));
  bool paused = false;
  bool leaderboard_launched = false;
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

  std::vector<std::vector<sf::Sprite>> revealed_tile_sprites(
      field.size(), std::vector<sf::Sprite>(field.at(0).size()));
  for (int i = 0; i < field.size(); i++) {
    for (int j = 0; j < field.at(0).size(); j++) {
      revealed_tile_sprites.at(i).at(j).setTexture(
          *textures.at("tile_revealed"));
      revealed_tile_sprites.at(i).at(j).setPosition(32 * j, 32 * i);
    }
  }

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
  auto [flags_first_digit_sprite, flags_second_digit_sprite,
        flags_third_digit_sprite] =
      set_flag_digits(textures, config.rows, flags_to_place);

  int seconds = 0;
  int minutes = 0;
  auto [min_first_digit_sprite, min_second_digit_sprite, sec_first_digit_sprite,
        sec_second_digit_sprite] =
      set_timer_digits(textures, config.rows, config.cols, minutes, seconds);

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

  std::vector<std::vector<sf::Sprite>> flag_sprites(
      field.size(), std::vector<sf::Sprite>(field.at(0).size()));
  for (int i = 0; i < field.size(); i++) {
    for (int j = 0; j < field.at(0).size(); j++) {
      flag_sprites.at(i).at(j).setTexture(*textures.at("flag"));
      flag_sprites.at(i).at(j).setPosition(32 * j, 32 * i);
    }
  }

  while (window.isOpen()) {
    tiles_revealed = 0;
    for (int i = 0; i < field.size(); i++) {
      for (int j = 0; j < field.at(0).size(); j++) {
        if (field.at(i).at(j).hidden == false) {
          tiles_revealed++;
        }
      }
    }

    if (flags_to_place == 0 &&
        tiles_revealed == config.rows * config.cols - config.mines &&
        raspidorasilo == false) {
      won = true;
      debug_on = true; // to draw the mines
      paused = true;
      face_happy_sprite.setTexture(*textures.at("face_win"));
      if (!ran_victory_stuff) {
        ran_victory_stuff = true;
        record_top(minutes, seconds, config);
      }
    } else if (raspidorasilo == true) {
      face_happy_sprite.setTexture(*textures.at("face_lose"));
      paused = true;
      for (int i = 0; i < field.size(); i++) {
        for (int j = 0; j < field.at(0).size(); j++) {
          if (field.at(i).at(j).hidden == true &&
              field.at(i).at(j).flagged == false &&
              field.at(i).at(j).has_mine) {
            field.at(i).at(j).hidden = false;
            tile_sprites.at(i).at(j).setTexture(*textures.at("tile_revealed"));
          }
          debug_on = true; // to draw the mines
        }
      }
    }

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      // right click
      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Right) {
        sf::Vector2f pixel_clicked =
            window.mapPixelToCoords(sf::Mouse::getPosition(window));
        // right clicked tile
        for (int i = 0; i < flag_sprites.size(); i++) {
          for (int j = 0; j < flag_sprites.at(0).size(); j++) {
            if (tile_sprites.at(i).at(j).getGlobalBounds().contains(
                    pixel_clicked) &&
                !won && !raspidorasilo && !paused) {
              if (field.at(i).at(j).hidden == true) {
                if (field.at(i).at(j).flagged == false) {
                  flags_to_place--;
                  field.at(i).at(j).flagged = true;
                } else if (field.at(i).at(j).flagged == true) {
                  flags_to_place++;
                  field.at(i).at(j).flagged = false;
                }
                auto digit_sprites =
                    set_flag_digits(textures, config.rows, flags_to_place);
                flags_first_digit_sprite = std::get<0>(digit_sprites);
                flags_second_digit_sprite = std::get<1>(digit_sprites);
                flags_third_digit_sprite = std::get<2>(digit_sprites);
              }
            }
          }
        }
      }

      // left click
      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f pixel_clicked =
            window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // clicked debug button
        if (debug_sprite.getGlobalBounds().contains(pixel_clicked) &&
            won == false && raspidorasilo == false) {
          if (debug_on == false) {
            debug_on = true;
          } else if (debug_on == true) {
            debug_on = false;
          }
        }

        // clicked play/pause button
        if (play_pause_sprite.getGlobalBounds().contains(pixel_clicked) &&
            !won && !raspidorasilo && !leaderboard_launched) {
          if (paused == false) {
            paused = true;
            seconds_before_pause += stop - start;
            seconds_after_pause = stop - stop; // idk how to write 0
            play_pause_sprite.setTexture(*textures.at("play"));
          } else if (paused == true) {
            paused = false;
            start = std::chrono::steady_clock::now();
            play_pause_sprite.setTexture(*textures.at("pause"));
          }
        }

        // clicked leaderboard button
        if (leaderboard_sprite.getGlobalBounds().contains(pixel_clicked) &&
            !leaderboard_launched) {
          bool was_paused = paused;
          if (!was_paused) {
            paused = true;
            seconds_before_pause += stop - start;
            seconds_after_pause = stop - stop; // idk how to write 0
          }
          leaderboard_launched = true;
          for (int i = 0; i < field.size(); i++) {
            for (int j = 0; j < field.at(0).size(); j++) {
              window.draw(revealed_tile_sprites.at(i).at(j));
            }
          }
          window.display();
          launch_leaderboard();
          leaderboard_launched = false;
          if (!was_paused) {
            paused = false;
            start = std::chrono::steady_clock::now();
          }
        }

        // clicked leaderboard button
        if (face_happy_sprite.getGlobalBounds().contains(pixel_clicked)) {
          window.close();
          return true;
        }

        // clicked tile
        for (int i = 0; i < tile_sprites.size(); i++) {
          for (int j = 0; j < tile_sprites.at(0).size(); j++) {
            if (tile_sprites.at(i).at(j).getGlobalBounds().contains(
                    pixel_clicked) &&
                !won && !raspidorasilo && !paused) {
              if (field.at(i).at(j).flagged == false) {
                tile_sprites.at(i).at(j).setTexture(
                    *textures.at("tile_revealed"));
                field.at(i).at(j).hidden = false;
                if (field.at(i).at(j).has_mine == false &&
                    field.at(i).at(j).mines_nearby == 0) {
                  clear_empty_tiles(field, textures, tile_sprites, i, j);
                } else if (field.at(i).at(j).has_mine == true) {
                  for (int i = 0; i < field.size(); i++) {
                    for (int j = 0; j < field.at(0).size(); j++) {
                      if (field.at(i).at(j).has_mine)
                        tile_sprites.at(i).at(j).setTexture(
                            *textures.at("tile_revealed"));
                    }
                  }
                  raspidorasilo = true;
                }
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
        if (field.at(i).at(j).hidden == false) {
          window.draw(mine_or_digit_sprites.at(i).at(j));
        }
        if (field.at(i).at(j).flagged == true) {
          window.draw(flag_sprites.at(i).at(j));
        }
      }
    }
    if (debug_on == true) {
      for (int i = 0; i < config.mines; i++) {
        window.draw(debug_mine_sprites.at(i));
      }
    }

    if (paused && !won && !raspidorasilo) {
      for (int i = 0; i < field.size(); i++) {
        for (int j = 0; j < field.at(0).size(); j++) {
          window.draw(revealed_tile_sprites.at(i).at(j));
        }
      }
    }

    if (!paused) {
      stop = std::chrono::steady_clock::now();
      seconds_after_pause = stop - start;
    }

    window.draw(face_happy_sprite);
    window.draw(debug_sprite);
    window.draw(play_pause_sprite);
    window.draw(leaderboard_sprite);
    window.draw(flags_first_digit_sprite);
    window.draw(flags_second_digit_sprite);
    window.draw(flags_third_digit_sprite);

    seconds =
        (std::chrono::duration_cast<std::chrono::seconds>(seconds_before_pause)
             .count() +
         std::chrono::duration_cast<std::chrono::seconds>(seconds_after_pause)
             .count()) %
        60;
    minutes =
        (std::chrono::duration_cast<std::chrono::seconds>(seconds_before_pause)
             .count() +
         std::chrono::duration_cast<std::chrono::seconds>(seconds_after_pause)
             .count()) /
        60;
    auto timer_sprites =
        set_timer_digits(textures, config.rows, config.cols, minutes, seconds);
    min_first_digit_sprite = std::get<0>(timer_sprites);
    min_second_digit_sprite = std::get<1>(timer_sprites);
    sec_first_digit_sprite = std::get<2>(timer_sprites);
    sec_second_digit_sprite = std::get<3>(timer_sprites);

    window.draw(min_first_digit_sprite);
    window.draw(min_second_digit_sprite);
    window.draw(sec_first_digit_sprite);
    window.draw(sec_second_digit_sprite);

    window.display();
  }

  for (auto &[name, ptr] : textures) {
    delete ptr;
  }
  textures.clear();
  return false;
}
