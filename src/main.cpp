#include "MainApp.hpp"

#include "LispSystem.hpp"
#include "LispAPI.hpp"

int main(int argc, char** argv) {
  //create application object and get our singleton
  MainApp* app = MainApp::get_instance();

  LispSystem* ecl = new LispSystem;
  ecl->initialise(argc, argv);
  LispAPI::register_functions();

  //i forget what I was testing here.
  //cl_object cl_ptr = LispAPI::send_pointer_address(ecl);
  //LispAPI::return_pointer_address(cl_ptr);
  
  app->framework.open_framework(argc, argv);
  app->framework.set_window_title("Untitled");

  app->window = app->framework.open_window();

  app->start();
  
  app->reset_instance();

  cl_shutdown();

  return 0;
}
