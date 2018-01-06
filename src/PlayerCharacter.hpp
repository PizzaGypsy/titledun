#pragma once

/*
  This class will be used to handle the creation of PlayerCharacter objects.
*/

#include "MainApp.hpp"

class PlayerCharacter {
public:
  PlayerCharacter();
  NodePath character;

  void load_player_model();

  void remove_nodes();
private:
  NodePath outline;
};
