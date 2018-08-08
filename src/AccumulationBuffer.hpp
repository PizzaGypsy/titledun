#pragma once

/*
  This class is for setting up render-to-texture for some interesting motion blur effects,
  emulating an actual accumulation buffer.
*/

#include "MainApp.hpp"
#include "FullscreenRenderCard.hpp"

class PandaAccum {
public:
  PandaAccum();
  ~PandaAccum();
  Texture* render_to_texture();
  void create();
  float fade_rate = 0.03; //0.05
  float render_delay = 0.25; //0.25
  float initial_alpha = 0.75; //0.75
  
private:
  PT(GraphicsOutput) accum_buffer = NULL;
  PT(GraphicsOutput) accum_buffer_two = NULL;
  PT(DisplayRegion) region = NULL;
  PT(DisplayRegion) region_two = NULL;

  FullscreenRenderCard* card1 = NULL;
  FullscreenRenderCard* card2 = NULL;

  bool change = false;
  
  NodePath np_camera;
  
  AsyncTask* motion_blur_task = NULL;
  AsyncTask* fade_task = NULL;
  static AsyncTask::DoneStatus motion_blur(GenericAsyncTask* task, void* data);
  static AsyncTask::DoneStatus fader(GenericAsyncTask* task, void* data);
};
