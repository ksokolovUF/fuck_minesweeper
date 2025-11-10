#include <SFML/Graphics.hpp>
#include <iostream>
void set_text(sf::Text &text, float x, float y);
void user_name_input(const sf::Event &event, std::string &name,
                     sf::RenderWindow &window);

void launch_welcome();
