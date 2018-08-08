/*
  All the GameState tasks live here, since it is quite a lot of code
  and likely will not need to be touched much.
*/

AsyncTask::DoneStatus GameState::move_player(GenericAsyncTask* task, void* data) {
  
  if (M_A->framework.all_windows_closed() == false) {
	//player_node.get_parent().get_node(0)->reset_prev_transform();
	NodePath pc_node = player_node.get_parent();
	float new_x, new_y, new_z, new_head;

	new_x = pc_node.get_x();
	new_y = pc_node.get_y();
	new_z = pc_node.get_z();

	new_head = pc_node.get_h();

	if (M_A->key_pressed[E_KEY] == true) {
	  new_x -= sin(DEG_TO_RAD * M_A->alpha) * 1.0;
	  new_y += cos(DEG_TO_RAD * M_A->alpha) * 1.0;
	  //new_z += sin(DEG_TO_RAD * m_a->beta) * 1.0;
	  new_head = M_A->alpha + 180;
	  
	} else if (M_A->key_pressed[D_KEY] == true) {
	  new_x += sin(DEG_TO_RAD * M_A->alpha) * 1.0;
	  new_y -= cos(DEG_TO_RAD * M_A->alpha) * 1.0;
	  //new_z -= sin(DEG_TO_RAD * m_a->beta) * 1.0;
	  new_head = M_A->alpha;
	  
	} if (M_A->key_pressed[S_KEY] == true) {
	  new_x -= cos(DEG_TO_RAD * M_A->alpha) * 1.0;
	  new_y -= sin(DEG_TO_RAD * M_A->alpha) * 1.0;

	  if (M_A->key_pressed[E_KEY]) {
		new_head = M_A->alpha - 135;
	  } else if (M_A->key_pressed[D_KEY]) {
		new_head = M_A->alpha - 45;
	  } else {
		new_head = M_A->alpha - 90;
	  }
	  
	} else if (M_A->key_pressed[F_KEY]) {
	  new_x += cos(DEG_TO_RAD * M_A->alpha) * 1.0;
	  new_y += sin(DEG_TO_RAD * M_A->alpha) * 1.0;

	  if(M_A->key_pressed[E_KEY]) {
		new_head = M_A->alpha + 135;
	  } else if (M_A->key_pressed[D_KEY]) {
		new_head = M_A->alpha + 45;
	  } else {
		new_head = M_A->alpha + 90;
	  }
	}
	pc_node.set_pos(new_x, new_y, new_z);
	pc_node.set_hpr(new_head, 0.0f, 0.0f);
	
	return(AsyncTask::DS_cont);
  } else {
	return(AsyncTask::DS_exit); //Stop this task if all our windows are closed.
  }
}

bool moving = true;
AsyncTask::DoneStatus GameState::move_player_noclip(GenericAsyncTask* task, void* data) {
  PlayerCharacter* pc_obj = (PlayerCharacter*)data;
  float speed = 0.5;
  
  if (M_A->framework.all_windows_closed() == false) {
	NodePath pc_node = player_node.get_parent();
	float new_x, new_y, new_z, new_head;

	new_x = pc_node.get_x();
	new_y = pc_node.get_y();
	new_z = pc_node.get_z();

	new_head = pc_node.get_h();

	if (M_A->key_pressed[E_KEY] == true) {
	  /*
		player_node.get_parent().
		//set_fluid_pos(
		set_pos(
		player_node.get_parent().get_pos().get_x(),
		player_node.get_parent().get_pos().get_y() - 1.0,
		player_node.get_parent().get_pos().get_z());*/

	  new_x -= sin(DEG_TO_RAD * M_A->alpha) * speed;
	  new_y += cos(DEG_TO_RAD * M_A->alpha) * speed;
	  new_z += sin(DEG_TO_RAD * M_A->beta) * speed;
	  new_head = M_A->alpha + 180;
	  
	} else if (M_A->key_pressed[D_KEY] == true) {
	  new_x += sin(DEG_TO_RAD * M_A->alpha) * speed;
	  new_y -= cos(DEG_TO_RAD * M_A->alpha) * speed;
	  new_z -= sin(DEG_TO_RAD * M_A->beta) * speed;
	  new_head = M_A->alpha;
	  
	} if (M_A->key_pressed[S_KEY] == true) {
	  new_x -= cos(DEG_TO_RAD * M_A->alpha) * speed;
	  new_y -= sin(DEG_TO_RAD * M_A->alpha) * speed;

	  if (M_A->key_pressed[E_KEY]) {
		new_head = M_A->alpha - 135;
	  } else if (M_A->key_pressed[D_KEY]) {
		new_head = M_A->alpha - 45;
	  } else {
		new_head = M_A->alpha - 90;
	  }
	  
	} else if (M_A->key_pressed[F_KEY]) {
	  new_x += cos(DEG_TO_RAD * M_A->alpha) * speed;
	  new_y += sin(DEG_TO_RAD * M_A->alpha) * speed;

	  if(M_A->key_pressed[E_KEY]) {
		new_head = M_A->alpha + 135;
	  } else if (M_A->key_pressed[D_KEY]) {
		new_head = M_A->alpha + 45;
	  } else {
		new_head = M_A->alpha + 90;
	  }
	}
	pc_node.set_pos(new_x, new_y, new_z);
	pc_node.set_hpr(new_head, 0.0f, 0.0f);

	if (moving != true) {
	  if (M_A->key_pressed[E_KEY] ||
		  M_A->key_pressed[S_KEY] ||
		  M_A->key_pressed[F_KEY] ||
		  M_A->key_pressed[D_KEY]) {
		moving = true;
		pc_obj->anim_collection.loop("hyena-run", true);
	  }
	} else if (!M_A->key_pressed[E_KEY] &&
		  !M_A->key_pressed[S_KEY] &&
		  !M_A->key_pressed[F_KEY] &&
		  !M_A->key_pressed[D_KEY]) {
	  moving = false;
	  pc_obj->anim_collection.pose("hyena-lay1", 1);
	}
	
	//std::cout << "PC Position: " << pc_node.get_pos() << std::endl;

	return(AsyncTask::DS_cont);
  } else {
	return(AsyncTask::DS_exit); //Stop this task if all our windows are closed.
  }
}

inline AsyncTask::DoneStatus GameState::cam_rot(GenericAsyncTask* task, void* data) {
  bool show_ui = false;
  
  int cursor_x, cursor_y;

  //check to see if all windows are closed. prevents a thread related seg-fault (tested: Linux x86_64).
  if (M_A->framework.all_windows_closed() == false) {

	if (M_A->mouse_pressed[C_MOUSE3] == false) {
	  M_A->props.set_cursor_hidden(false);
	  //m_a->window->get_graphics_window()->request_properties(m_a->props); //turns vsync off?
	  
	  M_A->cursor_x_pos = M_A->window->get_graphics_window()->get_pointer(0).get_x();
	  M_A->cursor_y_pos = M_A->window->get_graphics_window()->get_pointer(0).get_y();
	}

	if (M_A->mouse_pressed[C_MOUSE3] == true) {
	  M_A->props.set_cursor_hidden(true);
	  //m_a->window->get_graphics_window()->request_properties(m_a->props); //turns vsync off?
	  //get cursor position
	  cursor_x = M_A->window->get_graphics_window()->get_pointer(0).get_x();
	  cursor_y = M_A->window->get_graphics_window()->get_pointer(0).get_y();

	  //relative cursor movement
	  cursor_x -= M_A->cursor_x_pos;
	  cursor_y -= M_A->cursor_y_pos;

	  //update rotation angles
	  M_A->alpha -= cursor_x * 0.05;
	  M_A->beta -= cursor_y * 0.05;
	
	  //TODO: clamp when beta == 90.0

	  /*std::cout << "alpha: " << m_a->alpha << std::endl << "beta: " << m_a->beta << std::endl <<
		"cursor: " << cursor_x << ", " << cursor_y << std::endl <<
		"cursor_pos: " << m_a->cursor_x_pos << ", " << m_a->cursor_y_pos << std::endl;*/

	  //Position camera around a point i.e. orbit as we rotate.
	  if (player_node) {
		M_A->window->get_camera_group().set_pos(player_node,
																	LVecBase3(
																	  sin(M_A->alpha*DEG_TO_RAD) * 3,
																	  cos(M_A->alpha*DEG_TO_RAD) * -3,
																	  cos(M_A->beta*DEG_TO_RAD) * 1));
	  }

	  //apply rotation to camera
	  M_A->window->get_camera_group().set_hpr(M_A->alpha, M_A->beta, 0.0f);

	  //reset cursor position
	  M_A->window->get_graphics_window()->move_pointer(0, M_A->cursor_x_pos, M_A->cursor_y_pos);
	}
	
	if (M_A->key_pressed[ESC_KEY] == true) {
	  if (show_ui == false) {
		M_A->mm_button_np.show();
		M_A->exit_menu_np.show();
		
		show_ui = true;
		M_A->key_pressed[ESC_KEY] = false;
	  } else {
		M_A->mm_button_np.hide();
		M_A->exit_menu_np.hide();

		show_ui = false;
		M_A->key_pressed[ESC_KEY] = false;
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
  int cell_x = TerrainManager::get_cell_x(pc_node.get_pos().get_x());
  TerrainManager::r_cell_x = cell_x;
  //pc_node.get_pos().get_x() / terrains->terrain_map_size;
  
  int cell_y = TerrainManager::get_cell_y(pc_node.get_pos().get_y());
  TerrainManager::r_cell_y = cell_y;
  //pc_node.get_pos().get_y() / terrains->terrain_map_size;
  
  float elevation;

  try {
	GeoMipTerrain* closest_terrain = terrains->v_terrains
	  .at(cell_x)
	  .at(cell_y);
	if (closest_terrain != NULL) {
	  
	  //check to make sure we aren't trying to
	  //get the elevation to a terrain heightmap that isn't loaded yet.
	  if (closest_terrain->heightfield().is_valid()) {
		elevation = closest_terrain->get_elevation(
		  pc_node.get_pos().get_x() - (cell_x * terrains->terrain_map_size),
		  pc_node.get_pos().get_y() - (cell_y * terrains->terrain_map_size)) * 50;
	  } else {
		elevation = 1.0;
	  }

	  if (pc_node.get_z() <= elevation) {
		pc_node.set_z(elevation);
	  } else {
		pc_node.set_z(pc_node.get_z() - 0.25);
	  }
	} else {
	  pc_node.set_z(pc_node.get_z() - 0.25);
	}
  } catch (const std::out_of_range& oor) {
	std::cerr << "Out of range: " << oor.what() << std::endl;
  }
  
  return(AsyncTask::DS_cont);
}
