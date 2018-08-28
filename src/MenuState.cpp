#include "MenuState.hpp"
#include "RocketEventManager.hpp"
#include "TerrainCompiler.hpp"

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

MenuState::MenuState() {
  ui_callbacks();
}

void MenuState::enter() {
  rEventManager::LoadWindow("pandarocket");

  fullscreen_card = new CardMaker("fullscreen card");
  fullscreen_card->set_frame_fullscreen_quad();
  
  title_np = new NodePath(fullscreen_card->generate());
  title_np->set_texture(wood_tex, 0);
  title_np->reparent_to(M_A->window->get_render_2d());
}

void MenuState::exit() {
  title_np->remove_node();
}

void MenuState::ui_callbacks() {
  //here we create a lambda function and send it as an event callback.
  rEventManager::RegisterEventHandler("pandarocket", [](Rocket::Core::Event& event,
														const Rocket::Core::String& value){
													   if(value=="play"){
														 M_A->p_app_state_manager->
														   change_app_state(
															 M_A->p_app_state_manager->
															 find_by_name("GameState"));
													   }
													   else if(value=="chargen"){
														 M_A->p_app_state_manager->
														   change_app_state(
															 M_A->p_app_state_manager->
															 find_by_name("CharacterGen"));
													   }
													   else if(value=="genmaps") {
														 PNMImage* big_image = new PNMImage("big.png");
														 big_image->
														   set_color_type(PNMImage::ColorType::CT_grayscale);
														 TerrainCompiler::generate_heightmaps((*big_image),
																							  ecl_to_int(LispSystem::lisp("*terrain-map-size*"))-1);
														 delete big_image;
													   }
													   else if(value=="exit"){
														 M_A->p_app_state_manager->
														   change_app_state(
															 M_A->p_app_state_manager->
															 find_by_name("MenuState"));
														 
														 M_A->framework.set_exit_flag();
													   }
													 });
}
