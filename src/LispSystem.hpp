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
private:
};
