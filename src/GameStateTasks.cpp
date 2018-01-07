/*
  All the GameState tasks live here, since it is quite a lot of code
  and likely will not need to be touched much.
*/

AsyncTask::DoneStatus GameState::move_player(GenericAsyncTask* task, void* data) {
  MainApp* m_a = MainApp::get_instance();
  
  if (m_a->framework.all_windows_closed() == false) {
	//player_node.get_parent().get_node(0)->reset_prev_transform();
	NodePath pc_node = player_node.get_parent();
	float new_x, new_y, new_z, new_head;

	new_x = pc_node.get_x();
	new_y = pc_node.get_y();
	new_z = pc_node.get_z();

	new_head = pc_node.get_h();

	if (m_a->key_pressed[E_KEY] == true) {
	  new_x -= sin(DEG_TO_RAD * m_a->alpha) * 1.0;
	  new_y += cos(DEG_TO_RAD * m_a->alpha) * 1.0;
	  //new_z += sin(DEG_TO_RAD * m_a->beta) * 1.0;
	  new_head = m_a->alpha + 180;
	  
	} else if (m_a->key_pressed[D_KEY] == true) {
	  new_x += sin(DEG_TO_RAD * m_a->alpha) * 1.0;
	  new_y -= cos(DEG_TO_RAD * m_a->alpha) * 1.0;
	  //new_z -= sin(DEG_TO_RAD * m_a->beta) * 1.0;
	  new_head = m_a->alpha;
	  
	} if (m_a->key_pressed[S_KEY] == true) {
	  new_x -= cos(DEG_TO_RAD * m_a->alpha) * 1.0;
	  new_y -= sin(DEG_TO_RAD * m_a->alpha) * 1.0;

	  if (m_a->key_pressed[E_KEY]) {
		new_head = m_a->alpha - 135;
	  } else if (m_a->key_pressed[D_KEY]) {
		new_head = m_a->alpha - 45;
	  } else {
		new_head = m_a->alpha - 90;
	  }
	  
	} else if (m_a->key_pressed[F_KEY]) {
	  new_x += cos(DEG_TO_RAD * m_a->alpha) * 1.0;
	  new_y += sin(DEG_TO_RAD * m_a->alpha) * 1.0;

	  if(m_a->key_pressed[E_KEY]) {
		new_head = m_a->alpha + 135;
	  } else if (m_a->key_pressed[D_KEY]) {
		new_head = m_a->alpha + 45;
	  } else {
		new_head = m_a->alpha + 90;
	  }
	}
	pc_node.set_pos(new_x, new_y, new_z);
	pc_node.set_hpr(new_head, 0.0f, 0.0f);
	
	return(AsyncTask::DS_cont);
  } else {
	return(AsyncTask::DS_exit); //Stop this task if all our windows are closed.
  }
}

AsyncTask::DoneStatus GameState::move_player_noclip(GenericAsyncTask* task, void* data) {
  MainApp* m_a = MainApp::get_instance();
  
  if (m_a->framework.all_windows_closed() == false) {
	//player_node.get_parent().get_node(0)->reset_prev_transform();
	NodePath pc_node = player_node.get_parent();
	float new_x, new_y, new_z, new_head;

	new_x = pc_node.get_x();
	new_y = pc_node.get_y();
	new_z = pc_node.get_z();

	new_head = pc_node.get_h();

	if (m_a->key_pressed[E_KEY] == true) {
	  /*
		player_node.get_parent().
		//set_fluid_pos(
		set_pos(
		player_node.get_parent().get_pos().get_x(),
		player_node.get_parent().get_pos().get_y() - 1.0,
		player_node.get_parent().get_pos().get_z());*/

	  new_x -= sin(DEG_TO_RAD * m_a->alpha) * 1.0;
	  new_y += cos(DEG_TO_RAD * m_a->alpha) * 1.0;
	  new_z += sin(DEG_TO_RAD * m_a->beta) * 1.0;
	  new_head = m_a->alpha + 180;
	  
	} else if (m_a->key_pressed[D_KEY] == true) {
	  new_x += sin(DEG_TO_RAD * m_a->alpha) * 1.0;
	  new_y -= cos(DEG_TO_RAD * m_a->alpha) * 1.0;
	  new_z -= sin(DEG_TO_RAD * m_a->beta) * 1.0;
	  new_head = m_a->alpha;
	  
	} if (m_a->key_pressed[S_KEY] == true) {
	  new_x -= cos(DEG_TO_RAD * m_a->alpha) * 1.0;
	  new_y -= sin(DEG_TO_RAD * m_a->alpha) * 1.0;

	  if (m_a->key_pressed[E_KEY]) {
		new_head = m_a->alpha - 135;
	  } else if (m_a->key_pressed[D_KEY]) {
		new_head = m_a->alpha - 45;
	  } else {
		new_head = m_a->alpha - 90;
	  }
	  
	} else if (m_a->key_pressed[F_KEY]) {
	  new_x += cos(DEG_TO_RAD * m_a->alpha) * 1.0;
	  new_y += sin(DEG_TO_RAD * m_a->alpha) * 1.0;

	  if(m_a->key_pressed[E_KEY]) {
		new_head = m_a->alpha + 135;
	  } else if (m_a->key_pressed[D_KEY]) {
		new_head = m_a->alpha + 45;
	  } else {
		new_head = m_a->alpha + 90;
	  }
	}
	pc_node.set_pos(new_x, new_y, new_z);
	pc_node.set_hpr(new_head, 0.0f, 0.0f);
	
	std::cout << "PC Position: " << pc_node.get_pos() << std::endl;
	//std::cout << "Projected cell from position: " << round_cell(pc_node.get_pos().get_x() / 0x200) << "_" <<
	//round_cell(pc_node.get_pos().get_y() / 0x200) << std::endl;
	//std::string find_name = to_string(round_cell(pc_node.get_pos().get_x() / 0x200)) + "_" + to_string(round_cell(pc_node.get_pos().get_y() / 0x200));
	//std::cout << get_terrain_by_name(TerrainManager::get_instance()->vector_o_terrains, find_name).get_root() << std::endl;

	return(AsyncTask::DS_cont);
  } else {
	return(AsyncTask::DS_exit); //Stop this task if all our windows are closed.
  }
}

inline AsyncTask::DoneStatus GameState::cam_rot(GenericAsyncTask* task, void* data) {
  bool show_ui = false;
  
  int cursor_x, cursor_y;
  MainApp* m_a = MainApp::get_instance();

  //check to see if all windows are closed. prevents a thread related seg-fault (tested: Linux x86_64).
  if (m_a->framework.all_windows_closed() == false) {

	if (m_a->mouse_pressed[C_MOUSE3] == false) {
	  m_a->props.set_cursor_hidden(false);
	  //m_a->window->get_graphics_window()->request_properties(m_a->props); //turns vsync off?
	  
	  m_a->cursor_x_pos = m_a->window->get_graphics_window()->get_pointer(0).get_x();
	  m_a->cursor_y_pos = m_a->window->get_graphics_window()->get_pointer(0).get_y();
	}

	if (m_a->mouse_pressed[C_MOUSE3] == true) {
	  m_a->props.set_cursor_hidden(true);
	  //m_a->window->get_graphics_window()->request_properties(m_a->props); //turns vsync off?
	  //get cursor position
	  cursor_x = m_a->window->get_graphics_window()->get_pointer(0).get_x();
	  cursor_y = m_a->window->get_graphics_window()->get_pointer(0).get_y();

	  //relative cursor movement
	  cursor_x -= m_a->cursor_x_pos;
	  cursor_y -= m_a->cursor_y_pos;

	  //update rotation angles
	  m_a->alpha -= cursor_x * 0.05;
	  m_a->beta -= cursor_y * 0.05;
	
	  //TODO: clamp when beta == 90.0

	  /*std::cout << "alpha: " << m_a->alpha << std::endl << "beta: " << m_a->beta << std::endl <<
		"cursor: " << cursor_x << ", " << cursor_y << std::endl <<
		"cursor_pos: " << m_a->cursor_x_pos << ", " << m_a->cursor_y_pos << std::endl;*/

	  //Position camera around a point i.e. orbit as we rotate.
	  if (player_node) {
		MainApp::get_instance()->window->get_camera_group().set_pos(player_node,
																	LVecBase3(
																	  sin(m_a->alpha*DEG_TO_RAD) * 20,
																	  cos(MainApp::get_instance()->alpha*DEG_TO_RAD) * -20,
																	  cos(MainApp::get_instance()->beta*DEG_TO_RAD) * 10));
	  }

	  //apply rotation to camera
	  m_a->window->get_camera_group().set_hpr(m_a->alpha, m_a->beta, 0.0f);

	  //reset cursor position
	  m_a->window->get_graphics_window()->move_pointer(0, m_a->cursor_x_pos, m_a->cursor_y_pos);
	}
	
	if (m_a->key_pressed[ESC_KEY] == true) {
	  if (show_ui == false) {
		m_a->mm_button_np.show();
		m_a->exit_menu_np.show();
		
		show_ui = true;
		m_a->key_pressed[ESC_KEY] = false;
	  } else {
		m_a->mm_button_np.hide();
		m_a->exit_menu_np.hide();

		show_ui = false;
		m_a->key_pressed[ESC_KEY] = false;
	  }
	}

	return(AsyncTask::DS_cont);
	
  } else {
	return(AsyncTask::DS_exit); //Stop this task if all our windows are closed.
  }
}

AsyncTask::DoneStatus GameState::terrain_collisions(GenericAsyncTask* task, void* data) {
  TerrainManager* terrains = (TerrainManager*)data;
  NodePath pc_node = player_node.get_parent();
  int cell_x = pc_node.get_pos().get_x() / terrains->terrain_map_size;
  int cell_y = pc_node.get_pos().get_y() / terrains->terrain_map_size;
  float elevation;

  try {
	GeoMipTerrain* closest_terrain = terrains->v_terrains
	  .at(cell_x)
	  .at(cell_y);
	elevation = closest_terrain->get_elevation(
	  pc_node.get_pos().get_x() - (cell_x * terrains->terrain_map_size),
	  pc_node.get_pos().get_y() - (cell_y * terrains->terrain_map_size)) * 50;

	if (pc_node.get_z() <= elevation) {
	  pc_node.set_z(elevation);
	} else {
	  pc_node.set_z(pc_node.get_z() - 0.25);
	}
  } catch (const std::out_of_range& oor) {
	std::cerr << "Out of range: " << oor.what() << std::endl;
  }
  
  return(AsyncTask::DS_cont);
}
