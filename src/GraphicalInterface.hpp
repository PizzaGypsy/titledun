#pragma once

/*
  This class holds the static event functions to be called on UI elements.
*/

#include "MainApp.hpp"

class GraphicalInterface {
public:
  static void menu_controls(const Event* e, void* data);
  static void console_callback(const Event *e, void *data);
  static void main_menu_button_clicked(const Event *e, void *data);
  static void exit_button_clicked(const Event *e, void *data);
private:
};
