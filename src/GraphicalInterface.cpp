#include "GraphicalInterface.hpp"

bool show_console = false;
bool show_mmenu = false;
void GraphicalInterface::menu_controls(const Event* e, void* data) {
  //PGEntry* console_box=(PGEntry *) data;

  //tilde
  if (e->get_name() == "`") {
	if (show_console == false) {
	  MainApp::get_instance()->console_np.show();
	  show_console = true;
	}
	
	else if (show_console == true) {
	  MainApp::get_instance()->console_np.hide();
	  show_console = false;
	}
  }

  //escape
  else if (e->get_name() == "escape" && MainApp::get_instance()->main_menu_button != NULL) {
	if (show_mmenu == false) {
	  MainApp::get_instance()->mm_button_np.show();
	  MainApp::get_instance()->exit_menu_np.show();
	  show_mmenu = true;
	}
	else if (show_mmenu == true) {
	  MainApp::get_instance()->mm_button_np.hide();
	  MainApp::get_instance()->exit_menu_np.hide();
	  show_mmenu = false;
	}
  }
  
}

void GraphicalInterface::console_callback(const Event *e, void *data) {
  PGEntry* console_box=(PGEntry *)data;
  
  if (console_box->get_plain_text() != "") {
	MainApp::get_instance()->console_text->append_text(console_box->get_plain_text() + '\n');
  
	MainApp::get_instance()->console_text_np.set_pos(0, 0, MainApp::get_instance()->console_text->get_num_rows() - 1);
  
	std::cout << "text in console: " << console_box->get_plain_text() << std::endl;
	console_box->set_text("");
  }
}

void GraphicalInterface::main_menu_button_clicked(const Event *e, void *data) {
  //from some other state to MenuState
  MainApp::get_instance()->m_pAppStateManager->change_app_state(MainApp::get_instance()->
																m_pAppStateManager->find_by_name("MenuState"));
}

void GraphicalInterface::exit_button_clicked(const Event *e, void *data) {
  MainApp::get_instance()->framework.set_exit_flag();
}
