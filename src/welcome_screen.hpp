#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
void user_name_input(const sf::Event &event, std::string &name,
                     sf::RenderWindow &window);

void launch_welcome();
