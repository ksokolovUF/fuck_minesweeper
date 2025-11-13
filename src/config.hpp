#include <array>
class Config {
public:
  unsigned int cols;
  unsigned int rows;
  unsigned int mines;
  void read_config();
  static constexpr std::array<std::pair<int, int>, 8> NEIGHBOR_OFFSETS = {
      {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};
};
