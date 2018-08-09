#pragma once

#include "LispSystem.hpp"

class ModuleSystem {
public:
  static void load_module(const std::string& name);
  static void unload_module(const std::string& name);
  static cl_object get_module_list();
  static cl_object get_module_paths();
  //static std::vector<std::string> loaded_module;
private:
};
