#include "players/human-player.h"


bool isNumber(const std::string& str)
{
  return str.find_first_not_of("0123456789") == std::string::npos;
}

std::pair<int, int> human_player_t::make_move(const field_t &my_field, const field_t &enemy_field) {

  output_stream << "Your field:" << std::endl;
  output_stream << my_field << std::endl;
  output_stream << "Enemy's field" << std::endl;
  output_stream << enemy_field << std::endl;
  output_stream << "Input you turn! [Example 1A or 10B or 6J]" << std::endl;

  while (true) {
    int x = 0, y = 0;
    std::string s;

    std::getline(input_stream, s);

    if (s.size() == 3 || s.size() == 2) {

      if (s.size() == 2 && std::isdigit(s[0])){
        x = int(s[0] - '0');
      } else if (std::isdigit(s[0]) && std::isdigit(s[1])) {
        x = int(s[1] - '0') + 10 * int(s[0] - '0');
      }
      x--;

      y = int(s.back() - 'A');

      if (field_t::is_cell_valid(x, y)) {
        return std::make_pair(x, y);
      }
    }

    output_stream << "Incorrect move! [Correct example 1A or 10B or 6J]" << std::endl;
  }

}

void human_player_t::on_duplicate_move(int x, int y) {
  output_stream << x + 1 << (char)('A' + y) << " DUPLICATE! Repeat move!" << std::endl;
}

void human_player_t::on_miss(int x, int y) {
  output_stream << x + 1 << (char)('A' + y) << " MISS! Enemy's turn!" << std::endl;
}

void human_player_t::on_hit(int x, int y) {
  output_stream << x + 1 << (char)('A' + y) << " HIT! Your next move!" << std::endl;
}

void human_player_t::on_kill(int x, int y) {
  output_stream << x + 1 << (char)('A' + y) << " KILL! Your next move!" << std::endl;
}

void human_player_t::on_win() {
  output_stream << "You WIN!" << std::endl;
}

void human_player_t::on_lose() {
  output_stream << "You LOSE!" << std::endl;
}