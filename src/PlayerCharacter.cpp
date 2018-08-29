#include "PlayerCharacter.hpp"

PlayerCharacter::PlayerCharacter() {}

void PlayerCharacter::load_player_model() {
  /*
	character = M_A->window->load_model(
	M_A->framework.get_models(),
	"../media/models/panda-model");
  */
  character = M_A->window->load_model(
	M_A->framework.get_models(),
	"../media/models/hyena.egg");
  character.set_name("playermodel");
  character.set_texture(TexturePool::get_global_ptr()->load_texture("../media/textures/hyenatex.png",
																	0,
																	false));
  //character.set_scale(500);

  outline = M_A->window->load_model(
	M_A->framework.get_models(),
	"../media/models/hyena.egg");
  
  character.set_light_off();

  /*
  outline.reparent_to(character);
  outline.set_scale(1.02);
  outline.clear_texture();
  outline.set_pos(0, 0, 0);
  outline.set_shader_off();
  outline.set_render_mode_perspective(false);
  outline.set_light_off();
  outline.set_attrib(CullFaceAttrib::make(CullFaceAttrib::Mode::M_cull_counter_clockwise));
  outline.set_color(0, 0, 0, 1);
  */
}

void PlayerCharacter::load_anims() {
  std::string run_anim_name = "../media/models/";
  run_anim_name += "hyena";  //species;
  run_anim_name += "-run.egg";
  
  //M_A->window->load_model(character, "../media/models/hyena-lay1.egg");
  M_A->window->load_model(character, run_anim_name);
  
  auto_bind(character.node(), anim_collection);
  anim_collection.loop_all(false);

  anim_collection.store_anim(anim_collection.get_anim(0), "Armature.1");
  //anim_collection.store_anim(anim_collection.get_anim(1), "hyena-lay1");
  anim_collection.store_anim(anim_collection.get_anim(1), "hyena-run");
  
  if (anim_collection.get_num_anims() > 0) {
	for (int n=0 ; n<anim_collection.get_num_anims() ; n++) {
	  std::cout << "loaded anims: " << anim_collection.get_anim_name(n) << std::endl;
	}
  } else {
	std::cout << "no anims!" << std::endl;
  }
  
  anim_collection.unbind_anim(anim_collection.get_anim(0)->get_name());
  anim_collection.unbind_anim(anim_collection.get_anim(0)->get_name());
  //anim_collection.unbind_anim(anim_collection.get_anim(0)->get_name());
}

void PlayerCharacter::unload_anims() {
  anim_collection.stop_all();
  anim_collection.clear_anims();
}

void PlayerCharacter::remove_nodes() {
  unload_anims();
  
  character.get_child(0).remove_node();
  character.get_child(0).remove_node();
  character.get_child(0).remove_node();
  
  std::cout << "nodecollection size: " << character.get_children().size() << std::endl;
  //anim_collection.~AnimControlCollection();
  outline.clear();
  character.clear();
  outline.remove_node();
  character.remove_node();
}

