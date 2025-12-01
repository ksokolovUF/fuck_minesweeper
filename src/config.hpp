#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <string>

class Config {
public:
  unsigned int cols;
  unsigned int rows;
  unsigned int mines;
  static std::string name;
  void read_config();
  Config();
  static constexpr std::array<std::pair<int, int>, 8> NEIGHBOR_OFFSETS = {
      {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};
  static void set_text(sf::Text &text, float x, float y);
};
