#include "Controls.hpp"

float Controls::alpha = 0.0f;
float beta = 0.0f;

void Controls::define_keys() {
  M_A->framework.define_key("mouse1", "", &Controls::mouse_event_handler, NULL);
  M_A->framework.define_key("mouse2", "", &Controls::mouse_event_handler, NULL);
  M_A->framework.define_key("mouse3", "", &Controls::mouse_event_handler, NULL);

  M_A->framework.define_key("mouse1-up", "", &Controls::mouse_event_handler, NULL);
  M_A->framework.define_key("mouse2-up", "", &Controls::mouse_event_handler, NULL);
  M_A->framework.define_key("mouse3-up", "", &Controls::mouse_event_handler, NULL);

  M_A->framework.define_key("escape", "", &GraphicalInterface::menu_controls, NULL);

  M_A->framework.define_key("`", "", &GraphicalInterface::menu_controls, NULL);
  M_A->framework.define_key("`-up", "", &Controls::key_event_handler, NULL);

  M_A->framework.define_key("e", "", &Controls::key_event_handler, NULL);
  M_A->framework.define_key("e-up", "", &Controls::key_event_handler, NULL);

  M_A->framework.define_key("d", "", &Controls::key_event_handler, NULL);
  M_A->framework.define_key("d-up", "", &Controls::key_event_handler, NULL);

  M_A->framework.define_key("s", "", &Controls::key_event_handler, NULL);
  M_A->framework.define_key("s-up", "", &Controls::key_event_handler, NULL);

  M_A->framework.define_key("f", "", &Controls::key_event_handler, NULL);
  M_A->framework.define_key("f-up", "", &Controls::key_event_handler, NULL);
}

AsyncTask::DoneStatus Controls::move_player(GenericAsyncTask* task, void* data) {
  
}
