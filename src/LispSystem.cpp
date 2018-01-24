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

void LispSystem::initialise(int argc, char **argv) {
  //ecl_set_option(ECL_OPT_TRAP_SIGINT, false);
  cl_boot(argc, argv);
  env = ecl_process_env();
  GC_enable();
  atexit(cl_shutdown);

  lisp("(compile-file \"initrc.lisp\")");
  lisp("(load \"../cfg/initrc.lisp\")");

  std::cout << lisp("(foo)") << std::endl;

  std::cout << cl_safe_eval(c_string_to_object("(agaak)"), Cnil, 13) << std::endl;
  std::cout << cl_safe_eval(c_string_to_object("(foo)"), Cnil, 13) << std::endl;

  std::cout << "Value of *some-global*: " << ecl_to_int(lisp("*some-global*")) << std::endl;
  std::cout << si_coerce_to_base_string(lisp("*global-str*"))->base_string.self << std::endl;

  ECL_CATCH_ALL_BEGIN(env) {
    /*
     * Code that is protected. Uncaught lisp conditions, THROW,
     * signals such as SIGSEGV and SIGBUS may cause jump to
     * this region.
     */
	//cl_eval(c_string_to_object("(agaak)"));
  } ECL_CATCH_ALL_IF_CAUGHT {
    /*
     * If the exception, lisp condition or other control transfer
     * is caught, this code is executed.
     */
  	std::cout << "Signal caught" << std::endl;
} ECL_CATCH_ALL_END;
  /*
   * In all cases we exit here.
   */

}
