#pragma once

/*
  Big head class. It manages a lot, maybe a little too much. 
  Mostly internal stuff that requires global state for one reason or another.
  When you look into the singleton, it looks into you.
*/

#include <math.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <future>

#include <pandaFramework.h>
#include <pandaSystem.h>

#include <nodePathCollection.h>

#include <genericAsyncTask.h>
#include <asyncTaskManager.h>

#include <cIntervalManager.h>
#include <cLerpNodePathInterval.h>
#include <cMetaInterval.h>

#include <texturePool.h>

#include <load_prc_file.h>

#include <shader.h>
#include <ambientLight.h>
#include <directionalLight.h>
#include <pointLight.h>
#include <spotlight.h>
#include <lightRampAttrib.h>
#include <colorAttrib.h>

#include <geoMipTerrain.h>

#include <graphicsPipeSelection.h>

#include <mouseButton.h>
#include <keyboardButton.h>
#include <pgEntry.h>

#include <cardMaker.h>

#include <cullFaceAttrib.h>
#include <bitMask.h>
#include <collisionHandler.h>
#include <collisionHandlerQueue.h>
#include <pnmFileTypeRegistry.h>
#include <textureStagePool.h>
#include <auto_bind.h>
#include <animControlCollection.h>

#include "ModuleSystem.hpp"
#include "AppStateManager.hpp"
#include "GraphicalInterface.hpp"
#include "UtilFunctions.hpp"

#define M_A MainApp::get_instance()

class MainApp
{
public:
  ~MainApp();

  PandaFramework framework;
  WindowFramework *window;

  WindowProperties props;

  static MainApp* get_instance();
  
  static void reset_instance();

  void start();

  float alpha = 0.0f, beta = 0.0f;
  int cursor_x_pos = 0, cursor_y_pos = 0;

  bool mouse_pressed[3] = {false, false, false};
  bool key_pressed[6] = {false, false, false, false, false, false};

  AppStateManager* p_app_state_manager;

  //UI------------------------------------------
  PT(PGEntry) p_console_box;
  NodePath console_np;
  PT(TextNode) p_console_text;
  NodePath console_text_np;

  PT(PGButton) p_main_menu_button;
  NodePath mm_button_np;

  PT(PGButton) p_exit_menu_button;
  NodePath exit_menu_np;

private:
  static MainApp* p_instance;

  void setup_console();

  MainApp();
  MainApp& operator=(MainApp const&) {};
};

