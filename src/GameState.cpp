#include "GameState.hpp"

const float DEG_TO_RAD = 0.01745329252;
NodePath player_node;

#include "GameStateTasks.cpp"

//NodePath panda_actor;
GameState::GameState() {
  game_fog = NULL;
  p_terrains = NULL;
  p_sky = NULL;
  buffer = NULL;
}

void GameState::make_ui_elements() {
  //main menu button
  m_a->main_menu_button = new PGButton("Main Menu");
  m_a->main_menu_button->setup("Main Menu");

  m_a->mm_button_np = m_a->window->get_aspect_2d().attach_new_node(m_a->main_menu_button);
  m_a->mm_button_np.set_scale(0.05);
  m_a->mm_button_np.hide();

  //callback
  m_a->framework.define_key(m_a->main_menu_button->get_click_event(MouseButton::one()),
							"button press",
							&GraphicalInterface::main_menu_button_clicked,
							m_a->main_menu_button);

  //exit button
  m_a->exit_menu_button = new PGButton("Exit Button");
  m_a->exit_menu_button->setup("Exit");

  m_a->exit_menu_np = m_a->window->get_aspect_2d().attach_new_node(m_a->exit_menu_button);
  m_a->exit_menu_np.set_scale(0.05);
  m_a->exit_menu_np.set_pos(0, 0, -0.25);
  m_a->exit_menu_np.hide();

  //callback
  m_a->framework.define_key(m_a->exit_menu_button->get_click_event(MouseButton::one()),
							"button press",
							&GraphicalInterface::exit_button_clicked,
							m_a->exit_menu_button);
}

/*static*/ NodePath GameState::panda_actor;

void GameState::enter() {
  std::cout << "Entering gamestate" << std::endl;

  Controls::define_keys();
  make_ui_elements();

  if (!game_fog) {
	game_fog = new Fog("foggy");
	game_fog->set_color(0.5, 0, 1);
	game_fog->set_exp_density(0.001);
  }
  camera = MainApp::get_instance()->window->get_camera_group();

  if(p_terrains == NULL) {
	p_terrains = new TerrainManager();
	p_terrains->build_terrains();
  }
  if(p_sky == NULL) {
	p_sky = new SkyManager();
	p_sky->create_fog();
	p_sky->create_sky();
	p_sky->set_ambient_light();
	p_sky->create_sun();
  }

  //make_test_panda();
  pc_test();
  buffer = new PandaAccum();
  buffer->create();
  
  //shader_attempt();

  if (camera_rotation_task == NULL) {
	camera_rotation_task = new GenericAsyncTask("Rotates the camera", (&cam_rot), (void*) NULL);
	AsyncTaskManager::get_global_ptr()->add(camera_rotation_task);

	move_player_task = new GenericAsyncTask("Moves the player", (&move_player_noclip), (void*) NULL);
	AsyncTaskManager::get_global_ptr()->add(move_player_task);

	terrain_gravity_task = new GenericAsyncTask("Terrain and gravity", (&terrain_collisions), p_terrains);
	AsyncTaskManager::get_global_ptr()->add(terrain_gravity_task);
  }	
}

void GameState::pc_test() {
  ich_bin.load_player_model();
  ich_bin.character.set_scale(0.005);
  ich_bin.character.reparent_to(MainApp::get_instance()->window->get_render());
  
  player_node = MainApp::get_instance()->window->get_render().attach_new_node("player");
  player_node.set_scale(200);
  player_node.set_compass();
  player_node.reparent_to(ich_bin.character);
  
  camera.reparent_to(player_node);
}

AsyncTask::DoneStatus move_panda_task(GenericAsyncTask* task, void* data) {
  CIntervalManager::get_global_ptr()->step();

  return AsyncTask::DS_cont;
}

void GameState::make_test_panda() {
  GameState::panda_actor = MainApp::get_instance()->window->load_model(
	MainApp::get_instance()->framework.get_models(),
	"../media/models/panda-model");

  NodePath outline = MainApp::get_instance()->window->load_model(
	MainApp::get_instance()->framework.get_models(),
	"../media/models/panda-model");
  
  PT(Shader) shader_test = Shader::load(Shader::SL_GLSL, "../media/shaders/test1.vert", "../media/shaders/test1.frag", "");
  GameState::panda_actor.set_shader(shader_test);

  outline.reparent_to(GameState::panda_actor);
  outline.set_scale(1.02);
  outline.set_color(0, 0, 0, 1);
  outline.set_pos(0, 0, -3);
  outline.set_shader_off();
  outline.set_render_mode_thickness(2.0);
  outline.set_attrib(CullFaceAttrib::make(CullFaceAttrib::Mode::M_cull_counter_clockwise));
  
  //panda_actor.set_shader(my_shader);

  player_node = MainApp::get_instance()->window->get_render().attach_new_node("player");
  player_node.reparent_to(GameState::panda_actor);
  player_node.set_scale(200);
  player_node.set_compass();
  camera.reparent_to(player_node);

  GameState::panda_actor.set_scale(0.005);
  GameState::panda_actor.reparent_to(MainApp::get_instance()->window->get_render());

  //load walk anim
  MainApp::get_instance()->window->load_model(GameState::panda_actor, "../media/models/panda-walk4");
  MainApp::get_instance()->window->loop_animations(false);

    //create the lerp intervals needed to walk back and forth
  PT(CLerpNodePathInterval) pandaPosInterval1, pandaPosInterval2,
	pandaHprInterval1, pandaHprInterval2;
  pandaPosInterval1 = new CLerpNodePathInterval("pandaPosInterval1",
												13.0, CLerpInterval::BT_no_blend,
												true, false, GameState::panda_actor, NodePath());
  pandaPosInterval1->set_start_pos(LPoint3f(0, 10, 0));
  pandaPosInterval1->set_end_pos(LPoint3f(0, -10, 0));

  pandaPosInterval2 = new CLerpNodePathInterval("pandaPosInterval2",
												13.0, CLerpInterval::BT_no_blend,
												true, false, GameState::panda_actor, NodePath());
  pandaPosInterval2->set_start_pos(LPoint3f(0, -10, 0));
  pandaPosInterval2->set_end_pos(LPoint3f(0, 10, 0));

  pandaHprInterval1 = new CLerpNodePathInterval("pandaHprInterval1",
												3.0, CLerpInterval::BT_no_blend,
												true, false, GameState::panda_actor, NodePath());
  pandaHprInterval1->set_start_hpr(LPoint3f(0, 0, 0));
  pandaHprInterval1->set_end_hpr(LPoint3f(180, 0, 0));

  pandaHprInterval2 = new CLerpNodePathInterval("pandaHprInterval2",
												3.0, CLerpInterval::BT_no_blend,
												true, false, GameState::panda_actor, NodePath());
  pandaHprInterval2->set_start_hpr(LPoint3f(180, 0, 0));
  pandaHprInterval2->set_end_hpr(LPoint3f(0, 0, 0));

  //create and play the sequence that plays the intervals
  PT(CMetaInterval) pandaPace;
  pandaPace = new CMetaInterval("pandaPace");
  pandaPace->add_c_interval(pandaPosInterval1, 0,
							CMetaInterval::RS_previous_end);
  pandaPace->add_c_interval(pandaHprInterval1, 0,
							CMetaInterval::RS_previous_end);
  pandaPace->add_c_interval(pandaPosInterval2, 0,
							CMetaInterval::RS_previous_end);
  pandaPace->add_c_interval(pandaHprInterval2, 0,
							CMetaInterval::RS_previous_end);
  pandaPace->loop();

  AsyncTaskManager::get_global_ptr()->add(new GenericAsyncTask("Moves the panda",
															   &move_panda_task, (void*) NULL));

}

void GameState::shader_attempt() {
  NodePath temp_node = NodePath("temp node");
  temp_node.set_shader(Shader::load("../media/shaders/lightingGen.sha"));
  NodePath light = MainApp::get_instance()->window->get_render().attach_new_node("light");
  light.set_pos(30, -50, 0);
  MainApp::get_instance()->window->get_render().set_shader_input("light", light);

  
  GraphicsOutput* normals_buffer = MainApp::get_instance()->window->get_graphics_window()->make_texture_buffer("normalsBuffer", 0, 0);
  normals_buffer->set_clear_color(LVecBase4(0.5, 0.5, 0.5, 1));
  normals_buffer->set_sort(-100);
  //NodePath normals_camera = MainApp::get_instance()->window->make_camera();
  Camera normals_camera = Camera("normalsBuffer",  MainApp::get_instance()->window->get_camera(0)->get_lens());
  //normals_camera.node()->set_render(MainApp::get_instance()->window->get_render());
  normals_camera.set_scene(MainApp::get_instance()->window->get_render());

  temp_node = NodePath("temp node");
  //temp_node = MainApp::get_instance()->window->get_graphics_window()->get_texture_card(); //I... I saw a box! It looked like it had normals...!
  temp_node = normals_buffer->get_texture_card();
  temp_node.set_shader(Shader::load("../media/shaders/normalGen.sha", Shader::SL_Cg));
  normals_camera.set_initial_state(temp_node.get_state());
  normals_camera.set_active(true);
  normals_buffer->add_render_texture(temp_node.get_texture(), GraphicsOutput::RenderTextureMode::RTM_copy_texture);

  //panda_actor.set_shader(Shader::load("../media/shaders/normalGen.sha", Shader::SL_Cg));
  //camera.reparent_to(temp_node);

  NodePath drawn_scene = normals_buffer->get_texture_card();
  drawn_scene.set_transparency(TransparencyAttrib::M_alpha);
  drawn_scene.set_color(1, 1, 1, 0);

  
  PT(Shader) ink_gen = Shader::load("../media/shaders/vertedge.cg", Shader::SL_Cg);

  //PT(Shader) shader_test = Shader::load("../media/shaders/test1.glsl", Shader::SL_GLSL);
  
  //PT(Shader) ink_gen = Shader::load("../media/shaders/Silhouette.glsl", Shader::SL_GLSL);
  //drawn_scene.set_shader(Shader::load("../media/shaders/normalGen.sha", Shader::SL_Cg));
  
  drawn_scene.set_shader(ink_gen);
  drawn_scene.set_shader_input("separation", LVecBase4(0.001, 0, 0.001, 0));
  drawn_scene.set_shader_input("cutoff", LVecBase4(0.3));
  //drawn_scene.reparent_to(MainApp::get_instance()->window->get_render_2d());
  temp_node.reparent_to(MainApp::get_instance()->window->get_render_2d());
}

bool GameState::pause() {
  
}

void GameState::resume() {
  
}

void GameState::exit() {
  AsyncTaskManager::get_global_ptr()->remove(camera_rotation_task);
  camera_rotation_task = NULL;

  AsyncTaskManager::get_global_ptr()->remove(move_player_task);
  move_player_task = NULL;

  AsyncTaskManager::get_global_ptr()->remove(terrain_gravity_task);
  terrain_gravity_task = NULL;

  m_a->window->get_render().clear_fog();

  m_a->mm_button_np.remove_node();
  m_a->main_menu_button = NULL;

  m_a->exit_menu_np.remove_node();
  m_a->exit_menu_button = NULL;

  camera.reparent_to(MainApp::get_instance()->window->get_render());
  GameState::panda_actor.remove_node();
  player_node.remove_node();

  ich_bin.remove_nodes();

  delete p_terrains;
  p_terrains = NULL;
  delete p_sky;
  p_sky = NULL;

  delete buffer;
  buffer = NULL;
}

