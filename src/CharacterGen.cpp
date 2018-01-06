#include "CharacterGen.hpp"

CharacterGen::CharacterGen() {
  
}

NodePath* CharacterGen::return_player_character() {
  
}

void CharacterGen::scroll_clicked(const Event *ev, void *data) {
  PGSliderBar* current_button=(PGSliderBar *)data;
  current_button->set_value(current_button->get_value() + 1);
}

void CharacterGen::make_ui_elements() {
  exit_button = new PGButton("ExitButton");
  exit_button->setup("Main Menu");
  
  exit_np = m_a->window->get_aspect_2d().attach_new_node(exit_button);
  exit_np.set_scale(0.05);
  exit_np.set_pos(0, 0, -0.5);

  m_a->framework.define_key(exit_button->get_click_event(MouseButton::one()),
							"button press",
							&GraphicalInterface::main_menu_button_clicked,
							exit_button);

  slider1 = new PGSliderBar("slider1");
  slider1->setup_scroll_bar(false, 1, 1, 1);
  slider1->set_range(0, 255);
  slider1->set_scroll_size(4);
  
  slider1_np = m_a->window->get_aspect_2d().attach_new_node(slider1);
  slider1_np.set_scale(0.05);
  slider1_np.set_pos(0, 0, -0.25);

}

void CharacterGen::enter() {
  make_ui_elements();
}

void CharacterGen::exit() {
  exit_np.remove_node();
  slider1_np.remove_node();
}

bool CharacterGen::pause() {}
void CharacterGen::resume() {}
