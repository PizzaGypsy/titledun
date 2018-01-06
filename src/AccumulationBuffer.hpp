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
  
private:
  PT(GraphicsOutput) accum_buffer;
  PT(DisplayRegion) region;

  Texture* card_tex;

  CardMaker* fullscreen_card = new CardMaker("fullscreen");
  NodePath* fullscreen_node;
  
  AsyncTask *motion_blur_task = NULL;
  static AsyncTask::DoneStatus motion_blur(GenericAsyncTask* task, void* data);
};
