#include "MainApp.hpp"
#include "TerrainManager.hpp"

int main(int argc, char** argv) {
  //create application object and get our singleton
  MainApp* app = MainApp::get_instance();
  
  app->framework.open_framework(argc, argv);
  app->framework.set_window_title("Untitled");

  app->window = app->framework.open_window();

  app->start();

  app->reset_instance();

  return 0;
}
