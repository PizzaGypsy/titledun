#include "MainApp.hpp"

#include "MenuState.hpp"
#include "GameState.hpp"
#include "CharacterGen.hpp"

MainApp* MainApp::p_instance = NULL;

MainApp* MainApp::get_instance() {
  if (p_instance == NULL) {
	p_instance = new MainApp;
  }
  
  return p_instance;
}

void MainApp::reset_instance() {
  delete p_instance;
  p_instance = NULL;
}

MainApp::MainApp()
{
	p_app_state_manager = NULL;
	p_console_box = NULL;
	p_console_text = NULL;
	p_main_menu_button = NULL;
	p_exit_menu_button = NULL;
	std::fill(key_pressed, key_pressed+222, false);
}

MainApp::~MainApp()
{
  rEventManager::Shutdown();
  r_context->RemoveReference();
  Rocket::Core::Shutdown();
  
  p_app_state_manager->exit_app_state(p_app_state_manager->find_by_name("GameState"));
  delete p_app_state_manager;

  framework.close_framework();
}

void MainApp::setup_console() {
  p_console_box = new PGEntry("console");
  p_console_box->setup(15, 1);

  console_np = window->get_aspect_2d()
	.attach_new_node(p_console_box);
  
  console_np.set_scale(0.05);
  console_np.set_pos(-1, 0, -0.25);
  console_np.hide();

  p_console_text = new TextNode("console text");
  p_console_text->set_text("User Shell\n");
  p_console_text->set_wordwrap(15.0);
  //p_console_text->set_max_rows(5);
  
  console_text_np = window->get_aspect_2d()
	.attach_new_node(p_console_text);

  console_text_np.reparent_to(console_np);
  //console_text_np.set_scale(0.05);
  console_text_np.set_pos(0, 0, 1);

  framework.define_key(p_console_box->get_accept_event(KeyboardButton::enter()),
					   "accept",
					   &GraphicalInterface::console_callback,
					   p_console_box);
}

void MainApp::start()
{
  //unload_prc_file(ConfigPageManager::get_global_ptr()->get_implicit_page(0));
  PT(FrameRateMeter) meter = new FrameRateMeter("fps");
  meter->setup_window(window->get_graphics_output());

  props = window->get_graphics_window()->get_properties();
  window->get_graphics_window()->request_properties(props);

  //window->get_graphics_window()->set_clear_color(LColorf(0, 0, 0, 1));
  
  //window->get_render().set_shader_auto();
  //framework.set_perpixel(true);
  window->get_render().set_attrib(LightRampAttrib::make_double_threshold(0.0, 0.5, 0.2, 1));
  
  window->enable_keyboard();

  setup_console();

  p_app_state_manager = new AppStateManager();

  ModuleSystem::load_module("default");

  MenuState::create(p_app_state_manager, "MenuState");
  GameState::create(p_app_state_manager, "GameState");
  CharacterGen::create(p_app_state_manager, "CharacterGen");
  p_app_state_manager->start(p_app_state_manager->find_by_name("MenuState"));

  //framework.enable_default_keys();

  framework.main_loop();
}

void MainApp::setup_rocket() {
#ifdef HAVE_ROCKET_PYTHON
  Py_Initialize();
#endif
  r_region = RocketRegion::make("Panda Rocket", window->get_graphics_window());
  r_region->set_active(true);
  r_region->set_sort(10000);

  rih = new RocketInputHandler();
  window->get_mouse().attach_new_node(rih);
  r_region->set_input_handler(rih);

  Rocket::Controls::Initialise();

  r_context = r_region->get_context();
  Rocket::Debugger::Initialise(r_context);
  
  Rocket::Core::FontDatabase::LoadFontFace("../cfg/ui/Delicious-Roman.otf");

  r_instancer = new rEventInstancer();
  Rocket::Core::Factory::RegisterEventListenerInstancer(r_instancer);
  r_instancer->RemoveReference();

}
