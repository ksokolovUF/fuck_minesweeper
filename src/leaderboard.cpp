#include "leaderboard.hpp"
#include "config.hpp"
#include <fstream>

void launch_leaderboard() {
  Config config;
  const unsigned int window_height = config.rows * 16 + 50;
  const unsigned int window_length = config.cols * 16;
  sf::RenderWindow window(sf::VideoMode(window_height, window_length),
                          "SFML window", sf::Style::Close);
  sf::Font font;
  if (!font.loadFromFile("./files/font.ttf")) {
    throw "fucked up font path";
  }
  sf::Text leaderboard_text("LEADERBOARD", font, 20);
  leaderboard_text.setFillColor(sf::Color::White);
  Config::set_text(leaderboard_text, window_height / 2,
                   window_length / 2 - 120);
  leaderboard_text.setStyle(sf::Text::Bold);
  leaderboard_text.setStyle(sf::Text::Underlined);

  std::string top_str;
  std::ifstream stream("files/leaderboard.txt");
  if (stream) {
    std::string line;
    int index = 1;
    while (std::getline(stream, line)) {
      line = std::to_string(index) + ".\t" + line.substr(0, 5) + "\t" +
             line.substr(6);
      top_str += line;
      top_str += '\n';
      index++;
    }
  }

  sf::Text top_text(top_str, font, 18);
  top_text.setFillColor(sf::Color::White);
  Config::set_text(top_text, window_height / 2, window_length / 2 + 20);
  top_text.setStyle(sf::Text::Bold);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear(sf::Color::Blue);
    window.draw(leaderboard_text);
    window.draw(top_text);

    window.display();
  }
}
