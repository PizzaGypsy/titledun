#pragma once

#include <iostream>
#include <cstdlib>

#include <chrono>
#include <thread>
#include <ecl/ecl.h>

#define DEFUN(name,fun,args) \
  cl_def_c_function(c_string_to_object(name), \
					(cl_objectfn_fixed)fun, \
					args)

class LispSystem {
public:
  LispSystem();
  ~LispSystem();
  void initialise(int argc, char **argv);
  cl_env_ptr env;
  
  static cl_object lisp(const std::string &call);
  static std::string str_from_cl_struct(cl_object cl_struct_obj, cl_object obj_type, int idx);
  static std::string str_from_cl_obj(cl_object str_obj);
  static cl_object aref_2d(cl_object array, long unsigned int idx1, long unsigned int idx2);
private:
  static cl_object check_ql();
  static void install_ql();
  static void load_ql();
};
