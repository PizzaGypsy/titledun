#include "LispAPI.hpp"

SkyManager* LispAPI::p_sky = NULL;
TerrainManager* LispAPI::p_terr = NULL;

cl_object LispAPI::get_fog_color() {
  if (p_sky == NULL) {
	LispSystem::lisp("(format t \"WARNING: No registered SkyManager with LispAPI!~%\")");
	return(Cnil);
  }
  Fog* fog = p_sky->fog_node;
  
  //cons all the data into a list.
  cl_object list = cl_cons(ecl_make_singlefloat(fog->get_color().get_x()),
						   cl_cons(ecl_make_singlefloat(fog->get_color().get_y()),
								   cl_cons(ecl_make_singlefloat(fog->get_color().get_z()),
										   Cnil)));
  return(list);
}

cl_object LispAPI::set_fog_color(cl_object list) {
  if (p_sky == NULL) {
	LispSystem::lisp("(format t \"WARNING: No registered SkyManager with LispAPI!~%\")");
	return(Cnil);
  }
  p_sky->fog_node->set_color(ecl_to_float(ecl_car(list)),
							 ecl_to_float(ecl_car(ecl_cdr(list))),
							 ecl_to_float(ecl_car(ecl_cdr(ecl_cdr(list)))));
  return(LispSystem::lisp("t"));
}

void LispAPI::register_functions() {
  DEFUN("get-fog-color", get_fog_color, 0);
  DEFUN("set-fog-color", set_fog_color, 1);
  DEFUN("make-button", make_button, 1);
}

//register sky pointer with the API, so Lisp functions can get values from it.
void LispAPI::register_sky(SkyManager* sky) {
  p_sky = sky;
}

//Make it null so we can tell the API our pointer is invalid.
void LispAPI::unregister_sky() {
  p_sky = NULL;
}

cl_object LispAPI::send_pointer_address(void* ptr) {
  return(ecl_make_integer(reinterpret_cast<std::uintptr_t>(ptr)));
}

void* LispAPI::return_pointer_address(cl_object ptr) {
  //return(reinterpret_cast<void*>(ecl_to_uint(ptr)));
  return(reinterpret_cast<void*>(fix(ptr)));
}

cl_object LispAPI::cmd;
cl_object LispAPI::make_button(cl_object tmp) {
  //Let us get what we need for our button, first.

  cl_object obj_tag = c_string_to_object("ui-button");

  //Name lives on, in the NodePath.
  std::string name = LispSystem::str_from_cl_obj(ecl_structure_ref(tmp, obj_tag, 0));
  float posx = ecl_to_float(ecl_structure_ref(tmp, obj_tag, 1));
  float posy = ecl_to_float(ecl_structure_ref(tmp, obj_tag, 2));
  float scale = ecl_to_float(ecl_structure_ref(tmp, obj_tag, 3));
  cmd = ecl_structure_ref(tmp, obj_tag, 4);

  //Let us make it.
  
  PT(PGButton) btn = new PGButton(name);

  btn->setup(name);
  
  NodePath sum_node = M_A->window->get_aspect_2d().attach_new_node(btn);
  sum_node.set_scale(scale);
  sum_node.set_pos(posx, 0, posy);

  M_A->framework.define_key(btn->get_click_event(MouseButton::one()),
							"button press",
							&btn_fun,
							(void*)cmd);//squish

  return(Cnil);
}

void LispAPI::btn_fun(const Event* e, void* data) {
  cl_safe_eval(cmd, Cnil, Cnil);
}
