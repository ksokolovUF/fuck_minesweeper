struct Tile {
  bool has_mine = false;
  bool hidden = true;
  bool flagged = false;
  unsigned int mines_nearby = 0;
  unsigned int x = 0;
  unsigned int y = 0;
};

bool play_game();
