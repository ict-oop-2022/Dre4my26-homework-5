#pragma once

#include <memory>

#include "game/field.h"
#include "players/player-interface.h"

class sea_battle_t {
public:
  enum turn_t { FIRST_PLAYER = 0, SECOND_PLAYER = 1 };

  sea_battle_t(std::shared_ptr<player_interface_t> player1, field_t field1, std::shared_ptr<player_interface_t> player2, field_t field2):
        _player1(std::move(player1)), _player2(std::move(player2)), _field1(std::move(field1)), _field2(std::move(field2)) {};

  ~sea_battle_t() = default;

  void play();

  static turn_t change_turn(turn_t current_turn);
  static std::string get_player_name(turn_t turn);

private:
  std::shared_ptr<player_interface_t> _player1, _player2;
  field_t _field1, _field2;
};
