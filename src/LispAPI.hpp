#pragma once

/*
  A place where functions written in C++ but to be called from Lisp live,
  and how our Lisp scripts will interact with the engine.
*/

#include "MainApp.hpp"
#include "SkyManager.hpp"
#include "TerrainManager.hpp"
#include "LispSystem.hpp"

class LispAPI {
public:
  static cl_object get_fog_color();
  static cl_object set_fog_color(cl_object list);
  
  static void register_functions();

  static void register_sky(SkyManager* sky);
  static void unregister_sky();
  static cl_object send_pointer_address(void* ptr);
  static void* return_pointer_address(cl_object ptr);

  static cl_object make_button(cl_object tmp);

  static SkyManager* p_sky;
  static TerrainManager* p_terr;
  static void btn_fun(const Event* e, void* data);

  static cl_object cmd;
private:
};
