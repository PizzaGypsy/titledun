#include "LispSystem.hpp"

LispSystem::LispSystem() {}

LispSystem::~LispSystem() {}

cl_object LispSystem::lisp(const std::string &call) {
  return(cl_safe_eval(c_string_to_object(call.c_str()), Cnil, Cnil));
}

std::string LispSystem::str_from_cl_struct(cl_object cl_struct_obj, cl_object obj_type, int idx) {
  std::string str {""};

  cl_object tmp_bstr = si_coerce_to_base_string(
	ecl_structure_ref(cl_struct_obj, obj_type, idx));

  int cl_dim = tmp_bstr->string.dim;

  for (int i=0 ; i<cl_dim ; i++) {
	str += (*(tmp_bstr->string.self + 1));
  }
  return(str);
}

std::string LispSystem::str_from_cl_obj(cl_object str_obj) {
  std::string str {""};
  cl_object tmp_bstr = si_coerce_to_base_string(str_obj);
  int cl_dim = tmp_bstr->string.dim;

  for (int i=0 ; i<cl_dim ; i++) {
	str += (*(str_obj->string.self + i));
  }
  return(str);
}

cl_object LispSystem::aref_2d(cl_object array, long unsigned int idx1, long unsigned int idx2) {
  std::string call = {""};
  call += std::to_string(idx1);
  call += " ";
  call += std::to_string(idx2);

  cl_index cl_idx = ecl_to_index(lisp(call));

  return(ecl_aref(array, cl_idx));
}

void LispSystem::initialise(int argc, char **argv) {
  //ecl_set_option(ECL_OPT_TRAP_SIGINT, false);
  cl_boot(argc, argv);
  env = ecl_process_env();
  //atexit(cl_shutdown);

  /*
  if (check_ql() == Cnil) {
	install_ql();
  } else {
	load_ql();
	}*/

  lisp("(load \"../cfg/initrc.lisp\")");
}

cl_object LispSystem::check_ql() {
  cl_object result = lisp("(probe-file \"../cfg/quicklisp\")");
  return(result);
}

void LispSystem::install_ql() {
  lisp("(load \"../cfg/quicklisp.lisp\")");
  lisp("(quicklisp-quickstart:install :path \"../cfg/quicklisp\")");

  lisp("(load \"../cfg/quicklisp/setup.lisp\")");
  lisp("(ql:quickload \"cffi\")");
}

void LispSystem::load_ql() {
  lisp("(load \"../cfg/quicklisp/setup.lisp\")");
}
