#pragma once

/*
  Work in progress appstate for character generation.
*/

#include "MainApp.hpp"

class CharacterGen : public AppState {
public:

  DECLARE_APPSTATE_CLASS(CharacterGen)
  
  NodePath *player_char;
  
  CharacterGen();
  ~CharacterGen();

  void enter();
  void exit();
  bool pause();
  void resume();

  static NodePath* return_player_character();
  
private:
  MainApp* m_a = MainApp::get_instance();

  static void scroll_clicked(const Event *ev, void *data);
  
  void make_ui_elements();

  NodePath exit_np;
  PT(PGButton) exit_button;

  NodePath slider1_np;
  PT(PGSliderBar) slider1;
};
