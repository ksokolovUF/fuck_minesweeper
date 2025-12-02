#include "game.hpp"
#include "welcome_screen.hpp"
int main() {
  if (launch_welcome()) {
    while (play_game())
      ;
  }
  return 0;
}
