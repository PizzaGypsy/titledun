#include "GraphicalInterface.hpp"

bool show_console = false;
bool show_mmenu = false;
void GraphicalInterface::menu_controls(const Event* e, void* data) {
  //PGEntry* console_box=(PGEntry *) data;

  //tilde
  if (e->get_name() == "`") {
	if (show_console == false) {
	  M_A->console_np.show();
	  show_console = true;
	}
	
	else if (show_console == true) {
	  M_A->console_np.hide();
	  show_console = false;
	}
  }

  //escape
  else if (e->get_name() == "escape" && M_A->p_main_menu_button != NULL) {
	if (show_mmenu == false) {
	  M_A->mm_button_np.show();
	  M_A->exit_menu_np.show();
	  show_mmenu = true;
	}
	else if (show_mmenu == true) {
	  M_A->mm_button_np.hide();
	  M_A->exit_menu_np.hide();
	  show_mmenu = false;
	}
  }
  
}

void GraphicalInterface::console_callback(const Event *e, void *data) {
  PGEntry* p_console_box=(PGEntry *)data;
  
  if (p_console_box->get_plain_text() != "") {
	M_A->p_console_text->append_text(p_console_box->get_plain_text() + '\n');
  
	M_A->console_text_np.set_pos(0, 0, M_A->p_console_text->get_num_rows() - 1);
  
	std::cout << "text in console: " << p_console_box->get_plain_text() << std::endl;
	p_console_box->set_text("");
  }
}

void GraphicalInterface::main_menu_button_clicked(const Event *e, void *data) {
  //from some other state to MenuState
  M_A->p_app_state_manager->change_app_state(M_A->p_app_state_manager->
											 find_by_name("MenuState"));
}

void GraphicalInterface::exit_button_clicked(const Event *e, void *data) {
  //switch states first to ensure proper cleanup.
  M_A->p_app_state_manager->change_app_state(M_A->p_app_state_manager->
											 find_by_name("MenuState"));
  M_A->framework.set_exit_flag();
}
