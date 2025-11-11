struct Tile {
  bool has_bomb;
  unsigned int bombs_nearby;
  unsigned int x;
  unsigned int y;
};

void play_game();
