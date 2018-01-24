#include "TerrainManager.hpp"

inline AsyncTask::DoneStatus TerrainManager::update_terrain(GenericAsyncTask* task, void* data) {
  //iterate over vector of terrains and update LODs dynamically based on the camera's position.
  return AsyncTask::DS_cont;
}


TerrainManager::TerrainManager() {
  p_terrain_node = &terrain_objects_np;
}

TerrainManager::~TerrainManager() {
  AsyncTaskManager::get_global_ptr()->remove(AsyncTaskManager::get_global_ptr()->find_task("Updates terrain"));
  p_terrain_node->remove_node();
  
  for (int x=0 ; x<v_terrains.size() ; x++) {
	for (int y=0 ; y<v_terrains.size() ; y++) {
	  //v_terrains[x][y]->get_root().remove_node();
	  delete v_terrains[x][y];
	  std::cout << "Deleted Terrain" << x << "-" << y << std::endl;
	}
  }
  v_terrains.clear();
}

void TerrainManager::destroy_terrains() {
  AsyncTaskManager::get_global_ptr()->remove(AsyncTaskManager::get_global_ptr()->find_task("Updates terrain"));
  p_terrain_node->remove_node();
  
  for (int x=0 ; x<v_terrains.size() ; x++) {
	for (int y=0 ; y<v_terrains.size() ; y++) {
	  //v_terrains[x][y]->get_root().remove_node();
	  delete v_terrains[x][y];
	  std::cout << "Deleted Terrain" << x << "-" << y << std::endl;
	}
  }
  v_terrains.clear();
}

void TerrainManager::set_defaults(GeoMipTerrain* terrain, const std::string name, const std::string texture1, int x, int y) {
  
  std::string prefix = "../media/terrains/t";
  prefix += std::to_string(x);
  prefix += "_";
  prefix += std::to_string(y);
  prefix += ".png";
  terrain->set_heightfield(Filename(prefix));
  
  terrain->get_root().set_tex_scale(TextureStage::get_default(), 8, 8);

  //placeholder texture. TODO: add texture system
  Texture* wood_tex = TexturePool::get_global_ptr()->load_texture("../media/textures/wood_plain.jpg", 0, false);
  terrain->get_root().set_texture(wood_tex, 0);
  
  terrain->set_bruteforce(true);
  terrain->set_block_size(0x100);
  terrain->set_near(0x8);
  terrain->set_far(0x500);
  terrain->set_focal_point(MainApp::get_instance()->window->get_camera_group());
  //add terrain to terrain main node, for easy control of all terrain objects
  terrain->get_root().reparent_to(*(p_terrain_node));
  terrain->get_root().set_sz(50);
  terrain->get_root().set_pos((x * terrain_map_size), (y * terrain_map_size), 0);
  terrain->set_min_level(2);
  terrain->generate();
}

int TerrainManager::check_cfg_vars() {
  if (LispSystem::lisp("(length *terrains*)") == Cnil) {
	std::cout << "ERROR: '*terrains* undefined or empty" << std::endl;
	return(-1);
  } else if (LispSystem::lisp("*max-x*") == Cnil) {
	std::cout << "ERROR: '*max-x* undefined or nil" << std::endl;
	return(-1);
  } else if (LispSystem::lisp("*max-y*") == Cnil) {
	std::cout << "ERROR: '*max-y* undefined or nil" << std::endl;
	return(-1);
  }
  return(0);
}

int TerrainManager::parse_terrains_lisp() {
  if (check_cfg_vars() != 0) {
	return(-1);
  }
  int count = ecl_to_int(LispSystem::lisp("(length *terrains*)"));
  int max_x = ecl_to_int(LispSystem::lisp("*max-x*"));
  int max_y = ecl_to_int(LispSystem::lisp("*max-y*"));
  cl_object cl_terrains = LispSystem::lisp("*terrains*");

  std::cout << "max_x: " << max_x << std::endl << "max_y: " << max_y << std::endl;
  v_terrains.resize(max_x+1, std::vector<GeoMipTerrain*>(max_y+1, NULL));

  std::string name {""};
  std::string texture1 {""};
  int x {0};
  int y {0};
  cl_object obj_tag = c_string_to_object("terrain");
  cl_object one_terrain;
  
  for (int i=0 ; i<count ; i++) {
	one_terrain = ecl_car(cl_terrains);

	name = LispSystem::str_from_cl_struct(one_terrain, c_string_to_object("terrain"), 0);
	texture1 = LispSystem::str_from_cl_struct(one_terrain, c_string_to_object("terrain"), 1);
	
	x = ecl_to_int(
	  ecl_structure_ref(one_terrain, obj_tag, 2));
	y = ecl_to_int(
	  ecl_structure_ref(one_terrain, obj_tag, 3));

	try {
	  //If it's not null, that means there's something there.
	  //Do this check to avoid a dangling pointer.
	  if (v_terrains.at(x).at(y) == NULL) {
		p_terrain = new GeoMipTerrain(name);
		set_defaults(p_terrain, name, texture1, x, y);

		//add terrain to vector, so I can iterate over all instances as needed
		v_terrains.at(x).at(y) = p_terrain;
	  } else {
		std::cout << "WARNING: Attempted overwrite of pointer in v_terrains at ["
				  << x << "][" << y << "]" << std::endl
				  << "Aborting terrain cell creation." << std::endl;
	  }
	} catch (const std::out_of_range& oor) {
	  std::cerr << "Out of range error: " << oor.what() << std::endl;
	}

	std::cout << "Terrain loaded: " << count <<
	  "@x:" << x << " & y:" << y << std::endl;

	//Store the rest of the terrains for the next iteration.
	cl_terrains = ecl_cdr(cl_terrains);
  }
  std::cout << "v_terrains has size " << v_terrains.size() << std::endl;
  return(0);
}

void TerrainManager::build_terrains() {
  parse_terrains_lisp();
  //keep updating terrain w/ task
  //AsyncTaskManager::get_global_ptr()->add(new GenericAsyncTask("Updates terrain", (&update_terrain), (void*) NULL));

  p_terrain_node->show();
}

void TerrainManager::hide_terrains() {
  AsyncTaskManager::get_global_ptr()->remove(AsyncTaskManager::get_global_ptr()->find_task("Updates terrain"));
  p_terrain_node->hide();
}

void TerrainManager::enable_collisions() {
  BitMask32 good_mask = BitMask32(0x0);
  p_terrain_node->set_collide_mask(good_mask);
  
  p_queue = new CollisionHandlerQueue();
  traverser.add_collider(*(p_terrain_node), p_queue);
  traverser.traverse(MainApp::get_instance()->window->get_render());
 
  for(int i=0; i<p_queue->get_num_entries() ; ++i) {
	CollisionEntry *entry = p_queue->get_entry(i);
	std::cout << *entry << std::endl;
  }
}

