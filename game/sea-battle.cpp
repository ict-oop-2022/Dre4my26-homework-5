#include "game/sea-battle.h"

#include <utility>
#include <algorithm>


sea_battle_t::turn_t sea_battle_t::change_turn(turn_t current_turn) {
  return current_turn == FIRST_PLAYER ? SECOND_PLAYER : FIRST_PLAYER;
}

std::string sea_battle_t::get_player_name(turn_t turn) {
  return turn == FIRST_PLAYER ? "First" : "Second";
}

void sea_battle_t::play() {

  turn_t turn = FIRST_PLAYER;
  field_t field1 = _field1;
  field_t field2 = _field2;
  size_t ships_player1 = 0, ships_player2 = 0;
  std::pair<int, int> coords;
  int x = 0, y = 0;

  //count and replace ships
  for (size_t i = 0; i < field_t::FIELD_SIZE; i++) {
    for (size_t j = 0; j < field_t::FIELD_SIZE; j++) {
      if (field1[i][j] == field_t::SHIP_CELL) {
        ships_player1++;
        field1[i][j] = field_t::EMPTY_CELL;
      }
      if (field2[i][j] == field_t::SHIP_CELL) {
        ships_player2++;
        field2[i][j] = field_t::EMPTY_CELL;
      }
    }
  }

  while (true) {

    if (turn == FIRST_PLAYER) {
      coords = _player1->make_move(_field1, field2);

      if (!field_t::is_cell_valid(coords.first, coords.second)) {
        _player1->on_incorrect_move(coords.first, coords.second);
        continue;
      }

      if (field2[coords.first][coords.second] == field_t::HIT_CELL) {
        _player1->on_duplicate_move(coords.first, coords.second);
        continue;
      }

      if (_field2[coords.first][coords.second] == field_t::EMPTY_CELL) {
        field2[coords.first][coords.second] = field_t::MISS_CELL;
        _field2[coords.first][coords.second] = field_t::MISS_CELL;
        _player1->on_miss(coords.first, coords.second);

        // change turn
        turn = change_turn(turn);

        continue;
      }

      if (_field2[coords.first][coords.second] == field_t::SHIP_CELL) {
        ships_player2--;
        field2[coords.first][coords.second] = field_t::HIT_CELL;
        _field2[coords.first][coords.second] = field_t::HIT_CELL;

        bool has_neigh = false;

        for (auto i : field_t::DIRECTIONS) {
          x = coords.first;
          y = coords.second;

          while (x >= 0 && x < field_t::FIELD_SIZE && y >= 0 && y < field_t::FIELD_SIZE &&
                 (_field2[x][y] == field_t::SHIP_CELL || field2[x][y] == field_t::HIT_CELL)) {
            if (_field2[x][y] == field_t::SHIP_CELL) {
              has_neigh = true;
              break;
            }
            x += i.first;
            y += i.second;
          }
        }

        if (has_neigh) {
          _player1->on_hit(coords.first, coords.second);
        } else {
          _player1->on_kill(coords.first, coords.second);
        }
      }

    } else {

      coords = _player2->make_move(_field2, field1);

      if (!field_t::is_cell_valid(coords.first, coords.second)) {
        _player2->on_incorrect_move(coords.first, coords.second);
        continue;
      }

      if (field1[coords.first][coords.second] == field_t::HIT_CELL) {
        _player2->on_duplicate_move(coords.first, coords.second);
        continue;
      }

      if (_field1[coords.first][coords.second] == field_t::EMPTY_CELL) {
        field1[coords.first][coords.second] = field_t::MISS_CELL;
        _field1[coords.first][coords.second] = field_t::MISS_CELL;
        _player2->on_miss(coords.first, coords.second);

        // change turn
        turn = change_turn(turn);

        continue;
      }

      if (_field1[coords.first][coords.second] == field_t::SHIP_CELL) {
        ships_player1--;
        field1[coords.first][coords.second] = field_t::HIT_CELL;
        _field1[coords.first][coords.second] = field_t::HIT_CELL;

        bool has_neigh = false;

        for (auto i : field_t::DIRECTIONS) {
          x = coords.first;
          y = coords.second;

          while (x >= 0 && x < field_t::FIELD_SIZE && y >= 0 && y < field_t::FIELD_SIZE &&
                 (_field1[x][y] == field_t::SHIP_CELL || field1[x][y] == field_t::HIT_CELL)) {
            if (_field1[x][y] == field_t::SHIP_CELL) {
              has_neigh = true;
              break;
            }
            x += i.first;
            y += i.second;
          }
        }

        if (has_neigh) {
          _player2->on_hit(coords.first, coords.second);
        } else {
          _player2->on_kill(coords.first, coords.second);
        }
      }
    }

    //end game
    if (ships_player2 == 0) {
      _player1->on_win();
      _player2->on_lose();
      break;
    } else if (ships_player1 == 0) {
      _player2->on_win();
      _player1->on_lose();
      break;
    }
  }
}