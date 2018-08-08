#include "SkyManager.hpp"

inline AsyncTask::DoneStatus SkyManager::move_sky(GenericAsyncTask* task, void* data) {
  SkyManager* instance = (SkyManager*) data;
  //p_instance->sky_node->set_pos(M_A->window->get_camera_group().get_pos());
  
  //maybe improve this later?
  instance->sky_node->set_pos(M_A->window->get_camera_group().get_parent().get_parent().get_pos());

  return AsyncTask::DS_cont;
}

float sun_pitch = 0.0;
bool ascend = true;
inline AsyncTask::DoneStatus SkyManager::move_sun(GenericAsyncTask* task, void* data) {
  return AsyncTask::DS_cont;
}

SkyManager::SkyManager() {
  ambient_node = 0;
  sky_tex = 0;
  sky_node = 0;
  move_sky_task = 0;
  move_sun_task = 0;
  fog_node = 0;
  p_instance = this;
}

SkyManager::~SkyManager() {
  destroy_sky();
}

void SkyManager::create_sky() {
  if (sky_node == NULL) {
	sky = M_A->
	  window->load_model(M_A->framework.get_models(), "../media/models/skydome.egg");
	
	sky.look_at((PN_stdfloat)0.0, (PN_stdfloat)0.0, (PN_stdfloat)90.0);

	if (!sky_tex) {
	  //sky_tex = TexturePool::get_global_ptr()->load_texture("../media/textures/sky_twighlight_psphere.png", 0, false);
	  sky_tex = TexturePool::get_global_ptr()->load_texture("../media/textures/sky2.png", 0, false);
	}
	sky.set_texture(sky_tex, 0);

	sky.set_scale((PN_stdfloat)240.0); //60.0
	sky.set_bin("background", 0);
	sky.set_depth_write(false);
	sky.set_compass();
	sky.reparent_to(M_A->window->get_render());
	//sky.reparent_to(M_A->window->get_camera_group());
	sky.set_light_off(0);
	sky.set_fog_off();

	sky_node = &sky;

	if (move_sky_task == NULL) {
	  move_sky_task = new GenericAsyncTask("Move Sky", (&move_sky), p_instance);
	  AsyncTaskManager::get_global_ptr()->add(move_sky_task);
	}
  }
}

void SkyManager::create_sun() {
  //PT(DirectionalLight) d_light_sun;
  d_light_sun = new DirectionalLight("sun");
  d_light_sun->set_color(LVecBase4(0.4, 0.5, 1.0, 1.0));
  d_light_sun->set_shadow_caster(true, 0x200, 0x200);

  d_light_sun_np = M_A->window->get_render().attach_new_node(d_light_sun);
  d_light_sun_np.set_hpr(180, 260, 0);
  M_A->window->get_render().set_light(d_light_sun_np);
  sun_node = &d_light_sun_np;

  if (move_sun_task == NULL) {
	//move_sun_task = new GenericAsyncTask("Move Sun", (&move_sun), (void*) NULL);
	//move_sun_task->set_delay(0.5);
	//AsyncTaskManager::get_global_ptr()->add(move_sun_task);
  }
}

void SkyManager::set_ambient_light() {
  //PT(AmbientLight) a_light;
  a_light = new AmbientLight("ambient light");
  a_light->set_color(LVecBase4(0.2, 0.1, 0.1, 1));

  a_light_np = M_A->window->get_render().attach_new_node(a_light);
  M_A->window->get_render().set_light(a_light_np);

  ambient_node = &a_light_np;
}

void SkyManager::create_fog() {
  if (!fog_node) {
	fog_node = new Fog("foggy");
	//fog_node->set_color(0.5, 0, 1);
	fog_node->set_color(0.5, 0.5, 0.5);
	
	fog_node->set_mode(Fog::Mode::M_exponential);
	fog_node->set_exp_density(0.005);
  }
  M_A->window->get_render().set_fog(fog_node);
}

void SkyManager::destroy_sky() {
  if (move_sky_task) {
	AsyncTaskManager::get_global_ptr()->remove(AsyncTaskManager::get_global_ptr()->find_task("Move Sky"));
	//delete move_sky_task;
  }

  if (move_sun_task) {
	AsyncTaskManager::get_global_ptr()->remove(AsyncTaskManager::get_global_ptr()->find_task("Move Sun"));
	//delete move_sun_task;
	move_sun_task = NULL;
  }
  
  if (sky_node) {
	sky.remove_node();
	sky_node = NULL;
  }

  if (ambient_node) {
	M_A->window->get_render().clear_light();
	a_light_np.remove_node();
	ambient_node = NULL;
  }

  if (sun_node) {
	d_light_sun_np.remove_node();
	sun_node = NULL;
  }

  if (fog_node) {
  }

  if (sky_tex) {
  }
}
