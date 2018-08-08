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
  void load_anims();
  void unload_anims();

  void remove_nodes();
  AnimControlCollection anim_collection;
  std::string species {""};
private:
  NodePath outline;
};
