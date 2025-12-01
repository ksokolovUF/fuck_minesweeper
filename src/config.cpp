#include "config.hpp"
#include "fstream"
#include "iostream"

void Config::read_config() {
  std::ifstream ifile("files/config.cfg");
  std::string line;
  std::getline(ifile, line);
  cols = std::stoi(line);
  std::getline(ifile, line);
  rows = std::stoi(line);
  std::getline(ifile, line);
  mines = std::stoi(line);
  ifile.close();
}

Config::Config() { read_config(); }

std::string Config::name = "";

void Config::set_text(sf::Text &text, float x, float y) {
  sf::FloatRect textRect = text.getLocalBounds();
  text.setOrigin(textRect.left + textRect.width / 2.0f,
                 textRect.top + textRect.height / 2.0f);
  text.setPosition(sf::Vector2f(x, y));
}
