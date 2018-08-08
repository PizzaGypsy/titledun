#include "AccumulationBuffer.hpp"

PandaAccum::PandaAccum() {}

PandaAccum::~PandaAccum() {
  AsyncTaskManager::get_global_ptr()->remove(motion_blur_task);
  AsyncTaskManager::get_global_ptr()->remove(fade_task);
  
  accum_buffer->clear_render_textures();
  accum_buffer_two->clear_render_textures();
  accum_buffer->request_close();
  accum_buffer_two->request_close();
  accum_buffer->remove_all_display_regions();
  
  region->cleanup();
  region_two->cleanup();
  np_camera.clear();
  np_camera.remove_node();
  delete card1;
  delete card2;
}

AsyncTask::DoneStatus PandaAccum::motion_blur(GenericAsyncTask* task, void* data) {
  PandaAccum* object = (PandaAccum*)data;

  if (object->change == false) {
	object->accum_buffer->trigger_copy();
	object->card1->render_node->set_color(1, 1, 1, object->initial_alpha);
	object->change = true;
  } else if (object->change == true){
	object->accum_buffer_two->trigger_copy();
	object->card2->render_node->set_color(1, 1, 1, object->initial_alpha);
	object->change = false;
  }
   
  return(AsyncTask::DS_again);
}

AsyncTask::DoneStatus PandaAccum::fader(GenericAsyncTask* task, void* data) {
  PandaAccum* object = (PandaAccum*)data;
  float alpha_comp = object->card1->render_node->get_color().get_w();
  float alpha_comp_two = object->card2->render_node->get_color().get_w();
  
  alpha_comp -= object->fade_rate;
  alpha_comp_two -= object->fade_rate;

  if (alpha_comp <= 0.0) {
	alpha_comp = 0.0;
  }
  if (alpha_comp_two <= 0.0) {
	alpha_comp_two = 0.0;
  }

  object->card1->render_node->set_color(object->card1->render_node->get_color().get_x(),
										object->card1->render_node->get_color().get_y(),
										object->card1->render_node->get_color().get_z(),
										alpha_comp);

  object->card2->render_node->set_color(object->card2->render_node->get_color().get_x(),
										object->card2->render_node->get_color().get_y(),
										object->card2->render_node->get_color().get_z(),
										alpha_comp_two);
  
  return(AsyncTask::DS_again);
}

void PandaAccum::create() {
  card1 = new FullscreenRenderCard("buffA", 0.1);
  card2 = new FullscreenRenderCard("buffB", 0.6);
  
  accum_buffer = M_A->
	window->
	get_graphics_window()->
	make_texture_buffer("Accum", 1024, 1024);
  accum_buffer->set_sort(-100);
  accum_buffer->set_clear_color(LVecBase4(0, 0, 0, 0));

  accum_buffer_two = M_A->
	window->
	get_graphics_window()->
	make_texture_buffer("Accum2", 1024, 1024);
  accum_buffer_two->set_sort(-100);
  accum_buffer_two->set_clear_color(LVecBase4(0, 0, 0, 0));

  //Create a display region and give it a camera, so that the buffer has a scene to render.
  Camera* accum_camera = new Camera("accumCamera", M_A->window->get_camera(0)->get_lens());
  np_camera = M_A->window->get_render()
	.attach_new_node(accum_camera);
  np_camera.reparent_to(M_A->window->get_camera_group());
  
  region = accum_buffer->make_display_region();
  region->set_camera(np_camera);
  
  region_two = accum_buffer_two->make_display_region();
  region_two->set_camera(np_camera);

  //Render the contents of this buffer into the texture object.
  accum_buffer->add_render_texture(card1->render_tex,
								   GraphicsOutput::RenderTextureMode::RTM_triggered_copy_texture);

  accum_buffer_two->add_render_texture(card2->render_tex,
									   GraphicsOutput::RenderTextureMode::RTM_triggered_copy_texture);

  PandaAccum* p_instance = this;
  motion_blur_task = new GenericAsyncTask("motion blur", (&motion_blur), p_instance);
  fade_task = new GenericAsyncTask("fader", (&fader), p_instance);
  
  motion_blur_task->set_delay(render_delay);
  AsyncTaskManager::get_global_ptr()->add(motion_blur_task);
  AsyncTaskManager::get_global_ptr()->add(fade_task);
}

Texture* PandaAccum::render_to_texture() {
  if(accum_buffer) {
	return(accum_buffer->get_texture());
  } else {
	std::cout << "WARNING: accum_buffer empty!" << std::endl;
	return(NULL);
  }
}
