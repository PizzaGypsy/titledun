#include "ModuleSystem.hpp"

void ModuleSystem::load_module(const std::string& name) {
  std::string call {""};
  call += "(load \"../modules/";
  call += name;
  call += "/package.lisp\")";
  
  LispSystem::lisp(call);
}

void ModuleSystem::unload_module(const std::string& name) {
  std::string call {""};
  call += "(delete-package ";
  call += name;
  call += ")";
  
  LispSystem::lisp(call);
}

cl_object ModuleSystem::get_module_list() {
  return(LispSystem::lisp("(CORE:get-module-reg)"));
}

cl_object ModuleSystem::get_module_paths() {
  return(LispSystem::lisp("(probe-modules)"));
}
