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
