#include "PlayerCharacter.hpp"

PlayerCharacter::PlayerCharacter() {}

void PlayerCharacter::load_player_model() {
  character = MainApp::get_instance()->window->load_model(
	MainApp::get_instance()->framework.get_models(),
	"../media/models/panda-model");

  outline = MainApp::get_instance()->window->load_model(
	MainApp::get_instance()->framework.get_models(),
	"../media/models/panda-model");

  character.set_light_off();

  outline.reparent_to(character);
  outline.set_scale(1.02);
  outline.set_color(0, 0, 0, 1);
  outline.set_pos(0, 0, -3);
  outline.set_shader_off();
  outline.set_render_mode_perspective(false);
  outline.clear_texture();
  outline.set_light_off();
  outline.set_attrib(CullFaceAttrib::make(CullFaceAttrib::Mode::M_cull_counter_clockwise));
}

void PlayerCharacter::remove_nodes() {
  outline.remove_node();
  character.remove_node();
}

