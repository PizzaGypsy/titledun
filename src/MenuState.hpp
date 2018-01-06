#pragma once

/*
  This class builds the main menu you see at startup.
*/

#include "MainApp.hpp"
#include "AppState.hpp"

class MenuState : public AppState {
public:
  MenuState();

  DECLARE_APPSTATE_CLASS(MenuState)

  void enter();
  void exit();

private:
  static void play_clicked(const Event *ev, void *data);
  static void cgen_clicked(const Event *ev, void *data);

  PT(PGButton) play_button;
  NodePath play_np;

  CardMaker* fullscreen_card = new CardMaker("fullscreen card");
  NodePath *title_np = NULL;
  Texture* wood_tex = TexturePool::get_global_ptr()->load_texture("../media/textures/wood_plain.jpg", 0, false);

  PT(PGButton) exit_button;
  NodePath exit_np;

  PT(PGButton) cgen_button;
  NodePath cgen_np;
};
