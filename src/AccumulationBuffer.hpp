#pragma once

/*
  This class is for setting up render-to-texture for some interesting motion blur effects,
  emulating an actual accumulation buffer.
*/
#include "MainApp.hpp"

class PandaAccum {
public:
  PandaAccum();
  ~PandaAccum();
  Texture* render_to_texture();
  void create();
  NodePath temp_node;
  NodePath drawn_scene;
  PT(GraphicsOutput) accum_buffer;
  PT(DisplayRegion) region;

  CardMaker* fullscreen = new CardMaker("full");
  NodePath* full_node;
  
private:
  AsyncTask *motion_blur_task = NULL;
  static AsyncTask::DoneStatus motion_blur(GenericAsyncTask* task, void* data);
};
