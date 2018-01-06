#include "AccumulationBuffer.hpp"
//A buffer I intend on using for basic motion blur.

PandaAccum::PandaAccum() {
  accum_buffer = NULL;
  region = NULL;
}
PandaAccum::~PandaAccum() {
  region->cleanup();
  delete region;
  delete accum_buffer;
}

AsyncTask::DoneStatus PandaAccum::motion_blur(GenericAsyncTask* task, void* data) {
  PandaAccum* object = (PandaAccum*)data;
  //object->drawn_scene.set_texture(object->accum_buffer->get_texture());
  //object->drawn_scene = object->accum_buffer->get_texture_card();

  object->accum_buffer->trigger_copy();
   
  return(AsyncTask::DS_again);
}

void PandaAccum::create() {
  fullscreen->set_frame_fullscreen_quad();
  full_node = new NodePath(fullscreen->generate());
  full_node->set_transparency(TransparencyAttrib::M_alpha);
  full_node->set_color(1, 1, 1, 0.75);
  //full_node->set_texture(new Texture("buffd"));
  full_node->reparent_to(MainApp::get_instance()->window->get_render_2d());
  Texture* card_tex = new Texture("buffd");
  full_node->set_texture(card_tex);
  
  accum_buffer = MainApp::get_instance()->
	window->
	get_graphics_window()->
	make_texture_buffer("Accum", 1024, 1024);
  accum_buffer->set_sort(-100);
  accum_buffer->set_clear_color(LVecBase4(0.5, 0.5, 0.5, 1));

  region = accum_buffer->make_display_region();
  Camera* accum_camera = new Camera("accumCamera", MainApp::get_instance()->window->get_camera(0)->get_lens());
  NodePath np_camera = MainApp::get_instance()->window->get_render()
	.attach_new_node(accum_camera);
  region->set_camera(np_camera);
  np_camera.reparent_to(MainApp::get_instance()->window->get_camera_group());

  temp_node = NodePath("accumNode");
  //temp_node = MainApp::get_instance()->window->
  //get_graphics_window()->get_texture_card();
  //temp_node = accum_buffer->get_texture_card();
  temp_node.set_shader(Shader::load("../media/shaders/normalGen.sha", Shader::SL_Cg));
  temp_node.set_color(0, 1, 0, 0.5);
  //temp_node.set_transparency(TransparencyAttrib::M_alpha);
  temp_node.reparent_to(MainApp::get_instance()->window->get_render_2d());

  accum_buffer->add_render_texture(card_tex,
								   GraphicsOutput::RenderTextureMode::RTM_triggered_copy_texture);

  drawn_scene = accum_buffer->get_texture_card();//
  drawn_scene.set_transparency(TransparencyAttrib::M_alpha);
  drawn_scene.set_color(1, 0, 0, 0.75);
  //drawn_scene.set_texture(accum_buffer->get_texture());
  
  //drawn_scene.reparent_to(MainApp::get_instance()->window->get_render_2d());//

  PandaAccum* p_instance = this;
  motion_blur_task = new GenericAsyncTask("motion blur", (&motion_blur), p_instance);
  motion_blur_task->set_delay(0.5);
  AsyncTaskManager::get_global_ptr()->add(motion_blur_task);
}

Texture* PandaAccum::render_to_texture() {
  if(accum_buffer) {
	return(accum_buffer->get_texture());
  } else {
	std::cout << "WARNING: accum_buffer empty!" << std::endl;
	return(NULL);
  }
}
