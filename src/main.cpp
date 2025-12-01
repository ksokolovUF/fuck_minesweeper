#include "game.hpp"
#include "leaderboard.hpp"
#include "welcome_screen.hpp"
int main() {
  launch_welcome();
  while (play_game())
    ;
  launch_leaderboard();
  return 0;
}
