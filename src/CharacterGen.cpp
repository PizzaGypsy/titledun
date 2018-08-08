#include "CharacterGen.hpp"

CharacterGen::CharacterGen() {
}

//Updates all objects associated with sliders.
AsyncTask::DoneStatus CharacterGen::update_sliders(GenericAsyncTask* task, void* data) {
  CharacterGen* chargen =(CharacterGen*)data;
  
  chargen->bg_np->set_color(chargen->slider1->get_value() / 255,
							chargen->slider2->get_value() / 255,
							chargen->slider3->get_value() / 255);

  return(AsyncTask::DS_cont);
}

//I tried setting a 2d card for a background, it doesn't seem to work properly
void CharacterGen::setup_background() {
  fullscreen_card = new CardMaker("fullscreen card");
  fullscreen_card->set_frame_fullscreen_quad();

  bg_np = new NodePath(fullscreen_card->generate());
  bg_np->set_color(0, 0, 0);
  bg_np->set_bin("background", 0);
  bg_np->reparent_to(m_a->window->get_render_2d());
}

//makes the three colour sliders
void CharacterGen::make_sliders() {
  slider1 = new PGSliderBar("slider1");
  slider1->setup_slider(false, 1.9f, 0.1f, 0.005f);
  slider1->set_suppress_flags(MouseWatcherRegion::SF_mouse_button);
  slider1->get_thumb_button()->set_suppress_flags(MouseWatcherRegion::SF_mouse_button);

  slider1->set_range(0, 255);
  slider1->set_scroll_size(0.0f);
  slider1->set_page_size(1);

  slider1_np = m_a->window->get_aspect_2d().attach_new_node(slider1);
  slider1_np.set_scale(0.5);
  slider1_np.set_pos(-0.75, 0, 0.75);
  slider1_np.set_color(1, 0, 0);

  slider2 = new PGSliderBar("slider2");
  slider2->setup_slider(false, 1.9f, 0.1f, 0.005f);
  slider2->set_suppress_flags(MouseWatcherRegion::SF_mouse_button);
  slider2->get_thumb_button()->set_suppress_flags(MouseWatcherRegion::SF_mouse_button);

  slider2->set_range(0, 255);
  slider2->set_scroll_size(0.0f);
  slider2->set_page_size(1);

  slider2_np = m_a->window->get_aspect_2d().attach_new_node(slider2);
  slider2_np.set_scale(0.5);
  slider2_np.set_pos(-0.75, 0, 0.70);
  slider2_np.set_color(0, 1, 0);

  slider3 = new PGSliderBar("slider3");
  slider3->setup_slider(false, 1.9f, 0.1f, 0.005f);
  slider3->set_suppress_flags(MouseWatcherRegion::SF_mouse_button);
  slider3->get_thumb_button()->set_suppress_flags(MouseWatcherRegion::SF_mouse_button);

  slider3->set_range(0, 255);
  slider3->set_scroll_size(0.0f);
  slider3->set_page_size(1);

  slider3_np = m_a->window->get_aspect_2d().attach_new_node(slider3);
  slider3_np.set_scale(0.5);
  slider3_np.set_pos(-0.75, 0, 0.65);
  slider3_np.set_color(0, 0, 1);
}

//applys colour to the currently selected texture stage
void CharacterGen::apply_color(const Event *e, void* data) {
  CharacterGen* chargen=(CharacterGen*) data;

  chargen->v_layers.at(chargen->layer_idx)->set_color(LVector4f(
											   chargen->slider1->get_value() / 255,
											   chargen->slider2->get_value() / 255,
											   chargen->slider3->get_value() / 255,
											   1));

  chargen->v_text_layers.at(chargen->layer_idx)->set_text_color(LVector4f(
											   chargen->slider1->get_value() / 255,
											   chargen->slider2->get_value() / 255,
											   chargen->slider3->get_value() / 255,
											   1));
}

//makes buttons for obvious stuff
void CharacterGen::make_buttons() {
  //m_a->window->get_graphics_window()->set_clear_active(0, true);
  m_a->window->get_graphics_window()->set_clear_color_active(true);
  m_a->window->get_graphics_window()->set_clear_color(LColorf(1, 0, 0, 1));

  //exit
  exit_button = new PGButton("ExitButton");
  exit_button->setup("Main Menu");
  
  exit_np = m_a->window->get_aspect_2d().attach_new_node(exit_button);
  exit_np.set_scale(0.05);
  exit_np.set_pos(0, 0, -0.5);

  m_a->framework.define_key(exit_button->get_click_event(MouseButton::one()),
							"button press",
							&GraphicalInterface::main_menu_button_clicked,
							exit_button);

  //apply
  apply_button = new PGButton("ApplyButton");
  apply_button->setup("Apply");

  apply_np = m_a->window->get_aspect_2d().attach_new_node(apply_button);
  apply_np.set_scale(0.05);
  apply_np.set_pos(0, 0, -0.3);

  m_a->framework.define_key(apply_button->get_click_event(MouseButton::one()),
							"button press",
							&CharacterGen::apply_color,
							(void*)this);

  //previous
  previous_button = new PGButton("PreviousButton");
  previous_button->setup("<<");

  previous_np = m_a->window->get_aspect_2d().attach_new_node(previous_button);
  previous_np.set_scale(0.05);
  previous_np.set_pos(-0.9, 0, -0.3);

  m_a->framework.define_key(previous_button->get_click_event(MouseButton::one()),
							"button press",
							&CharacterGen::previous_layer,
							(void*)this);

  //next
  next_button = new PGButton("NextButton");
  next_button->setup(">>");

  next_np = m_a->window->get_aspect_2d().attach_new_node(next_button);
  next_np.set_scale(0.05);
  next_np.set_pos(-0.7, 0, -0.3);

  m_a->framework.define_key(next_button->get_click_event(MouseButton::one()),
							"button press",
							&CharacterGen::next_layer,
							(void*)this);
}

void CharacterGen::make_text() {
  //layer zero
  p_layer_zero = new TextNode("chargen layer0");
  p_layer_zero->set_text("Model Colour");
  
  p_layer_zero->set_shadow(0.05, 0.05);
  p_layer_zero->set_shadow_color(1, 1, 0, 1);
  p_layer_zero->set_text_color(0, 0, 0, 1);
  
  layer_zero_np = m_a->window->get_aspect_2d()
	.attach_new_node(p_layer_zero);
  layer_zero_np.set_pos(-0.90, 0, 0.3);
  layer_zero_np.set_scale(0.08);
  
  //layer one
  p_layer_one = new TextNode("chargen layer1");
  p_layer_one->set_text("Layer1 Colour");
  
  p_layer_one->set_shadow(0.05, 0.05);
  p_layer_one->set_shadow_color(1, 1, 1, 1);
  p_layer_one->set_text_color(0, 0, 0, 1);
  
  layer_one_np = m_a->window->get_aspect_2d()
	.attach_new_node(p_layer_one);
  layer_one_np.set_pos(-0.90, 0, 0.2);
  layer_one_np.set_scale(0.08);
}

void CharacterGen::make_ui_elements() {
  make_text();
  make_buttons();
  make_sliders();
}

void CharacterGen::enter() {
  make_ui_elements();
  setup_background();

  layer1 = new TextureStage("layer1");
  layer1->set_sort(1);
  layer1->set_mode(TextureStage::Mode::M_blend);
  initialise_data_structures();
  //layer1->set_color(LColor(1, 0, 0, 1));
  set_model();

  update_slider_task = new GenericAsyncTask("Updates chargen", (&update_sliders), (void*)this);
  AsyncTaskManager::get_global_ptr()->add(update_slider_task);

  camera = MainApp::get_instance()->window->get_camera_group();
  camera.set_pos(0, 0, 0);
  camera.set_hpr(0, 0, 0);
}

void CharacterGen::set_model() {
  /*
	player_char = m_a->window->load_model(
	MainApp::get_instance()->framework.get_models(),
	"../media/models/panda-model");
  */

  player_char = m_a->window->load_model(
	MainApp::get_instance()->framework.get_models(),
	"../media/models/hyena.egg");

  tex1 = TexturePool::get_global_ptr()->load_texture("../media/textures/cookie_cutter.png", 0, false);

  player_char.set_texture(layer1, tex1);

  //player_char.set_scale(0.005); //0.005
  player_char.set_pos(3, 35, -1);

  player_char.reparent_to(m_a->window->get_render());
  bg_np->hide();
}

void CharacterGen::load_species_list() {
  cl_object l_species = LispSystem::lisp("(get-species-list *species*)");
  int len = ecl_to_int(cl_list_length(l_species));
  sp_list.resize(len+1);

  for (int x=0 ; x<len ; x++) {
	cl_object element = ecl_car(l_species);
	sp_list.at(x) = LispSystem::str_from_cl_obj(element);

	l_species = ecl_cdr(l_species);
  }
}

int CharacterGen::load_markings() {
  cl_object result = LispSystem::lisp("(specie-idx \"hyena\" (get-species-list *species*))");
  int sp_idx = 0;
  if (result != Cnil) {
	sp_idx = ecl_to_int(result);
  }
  //build cmd string to get total number of markings
  std::string cmd = "(length (species-markings (nth ";
  cmd += std::to_string(sp_idx);
  cmd += " *species*)))";
  num_marks = ecl_to_int(LispSystem::lisp(cmd));
  
  //resize vector for number of marks
  v_markings.resize(num_marks+1, NULL);
  
  for (int x=0 ; x < num_marks ; x++) {
	
  }
  
  return(num_marks);
}

void CharacterGen::initialise_data_structures() {
  load_species_list();
  std::cout << "num markings" << load_markings() << std::endl;
  num_layers = 1;
  v_layers.resize(num_layers+1, NULL);
  v_text_layers.resize(num_layers+1, NULL);

  v_layers.at(0) = layer0;
  v_layers.at(1) = layer1;

  v_text_layers.at(0) = p_layer_zero;
  v_text_layers.at(1) = p_layer_one;
}

void CharacterGen::previous_layer(const Event* e, void* data) {
  CharacterGen* chargen=(CharacterGen*) data;
  
  if (chargen->layer_idx > 0) {
	chargen->v_text_layers.at(chargen->layer_idx)->set_shadow_color(1, 1, 1, 1);
	chargen->layer_idx--;
	chargen->v_text_layers.at(chargen->layer_idx)->set_shadow_color(1, 1, 0, 1);
  }
}

void CharacterGen::next_layer(const Event* e, void* data) {
  CharacterGen* chargen=(CharacterGen*) data;
  
  if (chargen->layer_idx < chargen->num_layers) {
	chargen->v_text_layers.at(chargen->layer_idx)->set_shadow_color(1, 1, 1, 1);
	chargen->layer_idx++;
	chargen->v_text_layers.at(chargen->layer_idx)->set_shadow_color(1, 1, 0, 1);
  }
}

void CharacterGen::exit() {
  AsyncTaskManager::get_global_ptr()->remove(update_slider_task);
  update_slider_task = NULL;

  layer_zero_np.remove_node();
  layer_one_np.remove_node();

  previous_np.remove_node();
  next_np.remove_node();
  
  bg_np->remove_node();
  exit_np.remove_node();
  apply_np.remove_node();
  slider1_np.remove_node();
  slider2_np.remove_node();
  slider3_np.remove_node();

  player_char.remove_node();
}

bool CharacterGen::pause() {}
void CharacterGen::resume() {}
