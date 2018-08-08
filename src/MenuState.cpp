#include "MenuState.hpp"

void MenuState::play_clicked(const Event *ev, void *data) {
  PGButton* CurrentButton=(PGButton *)data;
  
  printf("%s has been pressed.\n",CurrentButton->get_name().c_str());
  M_A->p_app_state_manager->
	change_app_state(M_A->p_app_state_manager->find_by_name("GameState"));
}

void MenuState::cgen_clicked(const Event *ev, void *data) {
  PGButton* current_button=(PGButton *)data;
  M_A->p_app_state_manager->
	change_app_state(M_A->p_app_state_manager->find_by_name("CharacterGen"));
}

MenuState::MenuState() {}

void MenuState::enter() {
  fullscreen_card = new CardMaker("fullscreen card");
  fullscreen_card->set_frame_fullscreen_quad();
  
  title_np = new NodePath(fullscreen_card->generate());
  title_np->set_texture(wood_tex, 0);
  title_np->reparent_to(M_A->window->get_render_2d());
  
  play_button = new PGButton("PlayButton");
  play_button->setup("PlayButton");

  play_np = M_A->window->get_aspect_2d().attach_new_node(play_button);
  play_np.set_scale(0.05);

  // Setup callback function
  M_A->framework.define_key(play_button->get_click_event(MouseButton::one()),
												"button press",
												&play_clicked,
												play_button);
  exit_button = new PGButton("ExitButton");
  exit_button->setup("Exit");

  exit_np = M_A->window->get_aspect_2d().attach_new_node(exit_button);
  exit_np.set_scale(0.05);
  exit_np.set_pos(0, 0, -0.25);

  M_A->framework.define_key(exit_button->get_click_event(MouseButton::one()),
												"button press",
												&GraphicalInterface::exit_button_clicked,
												exit_button);

  cgen_button = new PGButton("cGen");
  cgen_button->setup("Character Gen");

  cgen_np = M_A->window->get_aspect_2d().attach_new_node(cgen_button);
  cgen_np.set_scale(0.05);
  cgen_np.set_pos(0, 0, -0.1);

  M_A->framework.define_key(cgen_button->get_click_event(MouseButton::one()),
												"button press",
												&cgen_clicked,
												cgen_button);
}

void MenuState::exit() {
  play_np.remove_node();
  exit_np.remove_node();
  title_np->remove_node();
  cgen_np.remove_node();
}
