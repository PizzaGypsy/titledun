#include "AccumulationBuffer.hpp"

PandaAccum::PandaAccum() {
  accum_buffer = NULL;
  region = NULL;
  card_tex = NULL;
}
PandaAccum::~PandaAccum() {
  AsyncTaskManager::get_global_ptr()->remove(motion_blur_task);
  region->cleanup();
  fullscreen_node->remove_node();
}

AsyncTask::DoneStatus PandaAccum::motion_blur(GenericAsyncTask* task, void* data) {
  PandaAccum* object = (PandaAccum*)data;

  object->accum_buffer->trigger_copy();
   
  return(AsyncTask::DS_again);
}

void PandaAccum::create() {
  fullscreen_card->set_frame_fullscreen_quad();
  fullscreen_node = new NodePath(fullscreen_card->generate());
  fullscreen_node->set_transparency(TransparencyAttrib::M_alpha);
  fullscreen_node->set_color(1, 1, 1, 0.75);
  fullscreen_node->reparent_to(MainApp::get_instance()->window->get_render_2d());
  card_tex = new Texture("buffd");
  fullscreen_node->set_texture(card_tex);
  
  accum_buffer = MainApp::get_instance()->
	window->
	get_graphics_window()->
	make_texture_buffer("Accum", 1024, 1024);
  accum_buffer->set_sort(-100);
  accum_buffer->set_clear_color(LVecBase4(0.5, 0.5, 0.5, 1));

  //Create a display region and give it a camera, so that the buffer has a scene to render.
  region = accum_buffer->make_display_region();
  Camera* accum_camera = new Camera("accumCamera", MainApp::get_instance()->window->get_camera(0)->get_lens());
  NodePath np_camera = MainApp::get_instance()->window->get_render()
	.attach_new_node(accum_camera);
  region->set_camera(np_camera);
  np_camera.reparent_to(MainApp::get_instance()->window->get_camera_group());

  //Render the contents of this buffer into the texture object.
  accum_buffer->add_render_texture(card_tex,
								   GraphicsOutput::RenderTextureMode::RTM_triggered_copy_texture);

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
