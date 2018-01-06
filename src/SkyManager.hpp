#pragma once

/*
  This class handles ambient lighting, fog and making sure the skybox is centered on a node.
  Note, that it does not yet render a sun.
*/

#include "MainApp.hpp"

class SkyManager {
public:
  SkyManager();
  ~SkyManager();

  void create_sky();
  void create_sun();
  void set_ambient_light();
  void create_fog();
  
  void destroy_sky();

  NodePath *sky_node;
  NodePath *sun_node;
  NodePath *ambient_node;
  Fog *fog_node;
  
  Texture *sky_tex;
  //int sun_pitch;
  
  PT(DirectionalLight) d_light_sun;
  PT(AmbientLight) a_light;
  
private:
  NodePath sky;
  NodePath d_light_sun_np;
  NodePath a_light_np;

  static inline AsyncTask::DoneStatus move_sky(GenericAsyncTask* task, void* data);
  static inline AsyncTask::DoneStatus move_sun(GenericAsyncTask* task, void* data);
  
  AsyncTask *move_sky_task;
  AsyncTask *move_sun_task;
  
  SkyManager* p_instance;
  SkyManager& operator=(SkyManager const&) {};
};
