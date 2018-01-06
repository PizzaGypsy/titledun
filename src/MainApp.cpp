#include "MainApp.hpp"

#include "MenuState.hpp"
#include "GameState.hpp"
#include "CharacterGen.hpp"
//#include "PauseState.hpp"

MainApp* MainApp::m_pInstance = NULL;
libconfig::Config MainApp::cfg_parser;

MainApp* MainApp::get_instance() {
  if (m_pInstance == NULL) {
	m_pInstance = new MainApp;
  }
  
  return m_pInstance;
}

void MainApp::reset_instance() {
  delete m_pInstance;
  m_pInstance = NULL;
}

MainApp::MainApp()
{
	m_pAppStateManager = 0;
	console_box = 0;
	console_text = 0;
	main_menu_button = 0;
	exit_menu_button = 0;
}

MainApp::~MainApp()
{
  m_pAppStateManager->exit_app_state(m_pAppStateManager->find_by_name("GameState"));
  delete m_pAppStateManager;

  framework.close_framework();
}

void MainApp::setup_console() {
  console_box = new PGEntry("console");
  console_box->setup(15, 1);

  console_np = window->get_aspect_2d()
	.attach_new_node(console_box);
  
  console_np.set_scale(0.05);
  console_np.set_pos(-1, 0, -0.25);
  console_np.hide();

  console_text = new TextNode("console text");
  console_text->set_text("User Shell\n");
  console_text->set_wordwrap(15.0);
  //console_text->set_max_rows(5);
  
  console_text_np = window->get_aspect_2d()
	.attach_new_node(console_text);

  
  console_text_np.reparent_to(console_np);
  //console_text_np.set_scale(0.05);
  console_text_np.set_pos(0, 0, 1);

  framework.define_key(console_box->get_accept_event(KeyboardButton::enter()),
					   "accept",
					   &GraphicalInterface::console_callback,
					   console_box);
}

void MainApp::start()
{
  //unload_prc_file(ConfigPageManager::get_global_ptr()->get_implicit_page(0));
  PT(FrameRateMeter) meter = new FrameRateMeter("fps");
  meter->setup_window(window->get_graphics_output());

  props = window->get_graphics_window()->get_properties();
  window->get_graphics_window()->request_properties(props);

  //window->get_graphics_window()->set_clear_color(LColorf(0, 0, 0, 1));
  window->get_render().set_shader_auto();
  framework.set_perpixel(true);
  window->get_render().set_shader_auto();
  window->get_render().set_attrib(LightRampAttrib::make_double_threshold(0.0, 0.5, 0.2, 1));
  //window->get_render().set_color_off();
  //window->get_render().set_attrib(ColorAttrib::make_vertex());

  window->enable_keyboard();

  setup_console();

  m_pAppStateManager = new AppStateManager();

  MenuState::create(m_pAppStateManager, "MenuState");
  GameState::create(m_pAppStateManager, "GameState");
  CharacterGen::create(m_pAppStateManager, "CharacterGen");
  m_pAppStateManager->start(m_pAppStateManager->find_by_name("MenuState"));

  //framework.enable_default_keys();

  framework.main_loop();
}
