#include "welcome_screen.hpp"
#include "config.hpp"

void start_game(sf::RenderWindow window) { window.close(); }

bool user_name_input(const sf::Event &event, std::string &name,
                     sf::RenderWindow &window) {
  if (event.type != sf::Event::TextEntered) {
    return false;
  }
  const uint32_t u = event.text.unicode;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace) && !name.empty()) {
    name.pop_back();
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) &&
             name.size() >= 1) {
    Config::name = name;
    window.close();
    return true;
  } else if (std::isalpha(static_cast<char>(u))) {
    if (name.size() == 10) {
      return false;
    }
    if (name.empty()) {
      name += std::toupper(static_cast<char>(u));
    } else {
      name += std::tolower(static_cast<char>(u));
    }
  }
  return false;
}

bool launch_welcome() {
  Config config;
  const unsigned int window_height = config.rows * 32 + 100;
  const unsigned int window_length = config.cols * 32;
  sf::RenderWindow window(sf::VideoMode(window_length, window_height),
                          "SFML window", sf::Style::Close);
  sf::Font font;
  if (!font.loadFromFile("./files/font.ttf")) {
    throw "fucked up font path";
  }
  sf::Text welcome_text("WELCOME TO MINESWEEPER!", font, 24);
  welcome_text.setFillColor(sf::Color::White);
  welcome_text.setStyle(sf::Text::Bold);
  welcome_text.setStyle(sf::Text::Underlined);
  Config::set_text(welcome_text, window_length / 2, window_height / 2 - 150);
  sf::Text enter_name_text("Enter your name:", font, 20);
  enter_name_text.setFillColor(sf::Color::White);
  enter_name_text.setStyle(sf::Text::Bold);
  Config::set_text(enter_name_text, window_length / 2, window_height / 2 - 75);
  std::string name;
  sf::Text input_name_text("|", font, 18);
  input_name_text.setFillColor(sf::Color::Yellow);
  input_name_text.setStyle(sf::Text::Bold);
  Config::set_text(input_name_text, window_length / 2, window_height / 2 - 45);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (user_name_input(event, name, window))
        return true;
      input_name_text.setString(name + "|");

      if (event.type == sf::Event::Closed) {
        window.close();
        return false;
      }
    }

    window.clear(sf::Color::Blue);
    window.draw(welcome_text);
    window.draw(enter_name_text);

    Config::set_text(input_name_text, window_length / 2,
                     window_height / 2 - 45);
    window.draw(input_name_text);
    window.display();
  }
}
