#include "TerrainManager.hpp"

int TerrainManager::r_cell_x = 0;
int TerrainManager::r_cell_y = 0;

int TerrainManager::p_cell_x = 0;
int TerrainManager::p_cell_y = 0;

int TerrainManager::terrain_map_size = 1025;
bool TerrainManager::t_brute_force = true;
bool TerrainManager::t_border_stitch = true;
int TerrainManager::t_block_size = 256;
int TerrainManager::t_near_lod = 8;
int TerrainManager::t_far_lod = 1280;
int TerrainManager::t_min_q = 0;

void TerrainManager::cleanup_loader(GenericAsyncTask* task, bool b, void* data) {
  AsyncTaskManager::get_global_ptr()->remove(task);
}

AsyncTask::DoneStatus TerrainManager::update_lods(GenericAsyncTask* task, void* data) {
  TerrainManager* tm = (TerrainManager*)data;

  if (tm->p_b_world_threads->get_num_tasks() == 0) {
	for (int x=0 ; x<tm->v_terrains.size() ; x++) {
	  for (int y=0 ; y<tm->v_terrains.size() ; y++) {
		if (tm->v_terrains[x][y] != NULL) {
		  tm->v_terrains[x][y]->update();
		}
	  }
	}
  }
  return(AsyncTask::DS_cont);
}

Mutex* mut = new Mutex("locker");
AsyncTask::DoneStatus TerrainManager::set_defaults_async(GenericAsyncTask* task, void* data) {
  mut->acquire();
  t_container* tt = (t_container*)data;

  std::string prefix = "../media/terrains/t";
  prefix += std::to_string(tt->cell_x);
  prefix += "_";
  prefix += std::to_string(tt->cell_y);
  prefix += ".png";

  tt->p_terrain->set_heightfield(Filename(prefix));

  //for some reason, our heightmap gets rotated on the y-axis. Let's fix that.
  tt->p_terrain->heightfield().flip(false, true, false);
  
  tt->p_terrain->get_root().set_tex_scale(TextureStage::get_default(), 8, 8);

  //placeholder texture. TODO: add texture system
  Texture* wood_tex = TexturePool::get_global_ptr()->load_texture("../media/textures/wood_plain.jpg", 0, false);
  tt->p_terrain->get_root().set_texture(wood_tex, 0);

  tt->p_terrain->get_root().set_sz(50);
  tt->p_terrain->set_bruteforce(t_brute_force);
  tt->p_terrain->set_border_stitching(t_border_stitch);
  tt->p_terrain->set_block_size(t_block_size);
  tt->p_terrain->set_near(t_near_lod);
  tt->p_terrain->set_far(t_far_lod);
  tt->p_terrain->set_min_level(t_min_q);
  
  tt->p_terrain->set_focal_point(M_A->window->get_camera_group());
  tt->p_terrain->get_root().set_pos((tt->cell_x * (terrain_map_size-1)), (tt->cell_y * (terrain_map_size-1)), 0);
  
  tt->p_terrain->generate();

  delete tt;
  mut->release();

  return(AsyncTask::DS_done);
}

AsyncTask::DoneStatus TerrainManager::update_terrain(GenericAsyncTask* task, void* data) {
  TerrainManager* terrains = (TerrainManager*)data;
  
  if (p_cell_x != TerrainManager::r_cell_x || p_cell_y != TerrainManager::r_cell_y) {
	if (terrains->p_b_world_threads->get_num_tasks() == 0) {
	  terrains->purge_cell_buffer();
	  p_cell_x = TerrainManager::r_cell_x;
	  p_cell_y = TerrainManager::r_cell_y;
	}
	terrains->load_cells_nearby();
  }
  return(AsyncTask::DS_cont);
}


TerrainManager::TerrainManager() {
  p_terrain_node = &terrain_objects_np;
  p_b_world_threads = AsyncTaskManager::get_global_ptr()->make_task_chain("worldThreads");
  p_b_world_threads->set_num_threads(1);
  p_b_world_threads->set_frame_budget(0.05);
  p_b_world_threads->set_thread_priority(TP_low);
  get_terrain_settings();
}

TerrainManager::~TerrainManager() {
  AsyncTaskManager::get_global_ptr()->remove(AsyncTaskManager::get_global_ptr()->find_task("Updates terrain"));
  while (p_b_world_threads->get_num_tasks() != 0) {
	p_b_world_threads->wait_for_tasks();
	M_A->framework.do_frame(Thread::get_current_thread());
  }
  //p_b_world_threads->wait_for_tasks();
  AsyncTaskManager::get_global_ptr()->remove_task_chain("worldThreads");
  
  p_terrain_node->remove_node();
  
  for (int x=0 ; x<v_terrains.size() ; x++) {
	for (int y=0 ; y<v_terrains.size() ; y++) {
	  if (v_terrains[x][y] != NULL) {
		v_terrains[x][y]->get_root().remove_node();
		delete v_terrains[x][y];
		std::cout << "Deleted Terrain" << x << "-" << y << std::endl;
	  }
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
  terrain->set_focal_point(M_A->window->get_camera_group());
  terrain->get_root().set_sz(50);
  terrain->get_root().set_pos((x * terrain_map_size), (y * terrain_map_size), 0);
  terrain->set_min_level(2);
  terrain->generate();
}

int TerrainManager::check_cfg_vars() {
  if (LispSystem::lisp("(length *terrains*)") == Cnil
	  && LispSystem::lisp("(array-dimension *terrains* 0)") == Cnil) {
	
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

int TerrainManager::load_cell(int cell_x, int cell_y) {
  if (cell_x < 0) {
	cell_x = 0;
  } if (cell_y < 0) {
	cell_y = 0;
  }
  int max_x = ecl_to_int(LispSystem::lisp("*max-x*"));
  int max_y = ecl_to_int(LispSystem::lisp("*max-y*"));
  
  if (cell_x > max_x || cell_y > max_y) {
	return(2);
  }
  
  if (v_terrains.at(cell_x).at(cell_y) != NULL) {
	return(1);
  }

  std::string name = "";
  std::string texture1 = "";
  int x = 0;
  int y = 0;
  cl_object obj_tag = c_string_to_object("terrain");
  
  cl_object one_terrain = Cnil;
  
  //one_terrain = ecl_car(cl_terrains);
  one_terrain = LispSystem::aref_2d(LispSystem::lisp("*terrains*"), cell_x, cell_y);

  name = LispSystem::str_from_cl_struct(one_terrain, obj_tag, 0);
  texture1 = LispSystem::str_from_cl_struct(one_terrain, obj_tag, 1);
	
  x = ecl_to_int(
	ecl_structure_ref(one_terrain, obj_tag, 2));
  y = ecl_to_int(
	ecl_structure_ref(one_terrain, obj_tag, 3));

  try {
	//If it's not null, that means there's something there.
	//Do this check to avoid a dangling pointer.
	if (v_terrains.at(cell_x).at(cell_y) == NULL) {
	  p_terrain = new GeoMipTerrain(name);

	  t_container* tt = new t_container(p_terrain, name, texture1, cell_x, cell_y);

	  GenericAsyncTask* loader_task = new GenericAsyncTask(name, (&set_defaults_async), tt);
	  loader_task->set_upon_death(&cleanup_loader);
	  loader_task->set_task_chain("worldThreads");
	  AsyncTaskManager::get_global_ptr()->add(loader_task);

	  //none of these worked
	  //std::async(std::launch::async, set_defaults, p_terrain, name, texture1, cell_x, cell_y);
	  //std::thread t(set_defaults, p_terrain, name, texture1, cell_x, cell_y);
	  //t.join();
	  //set_defaults(p_terrain, name, texture1, cell_x, cell_y);

	  p_terrain->get_root().reparent_to(*(p_terrain_node));
	  //add terrain to vector, so I can iterate over all instances as needed
	  v_terrains.at(cell_x).at(cell_y) = p_terrain;
	  std::cout << "Created terrain at " << cell_x << " " << cell_y << std::endl;
	} else {
	  std::cout << "Skipping cell in v_terrains at ["
				<< x << "][" << y << "]" << std::endl;
	}
  } catch (const std::out_of_range& oor) {
	std::cerr << "Out of range error: " << oor.what() << std::endl;
  }
}

int TerrainManager::load_cells_nearby() {
  for (int ix= -1 ; ix <= 1 ; ix++) {
	for (int iy= -1 ; iy <= 1 ; iy++) {
	  load_cell(r_cell_x + ix, r_cell_y + iy);
	}
  }
  return(0);
}

void TerrainManager::purge_cell_buffer() {
  int cells_to_load = 1;

  for (int x=0 ; x<v_terrains.size() ; x++) {
	for (int y=0 ; y<v_terrains.size() ; y++) {
	  if (v_terrains[x][y] == NULL) {
		std::cout << "NULL@" << x << "_" << y << std::endl;
	  } else {
		//BIG BLOB OF IF that only purges cells that aren't one of the nine nearby.
		if (x == r_cell_x && y == r_cell_y
			|| x == r_cell_x - 1 && y == r_cell_y - 1
			|| x == r_cell_x && y == r_cell_y - 1
			|| x == r_cell_x + 1 && y == r_cell_y - 1
			|| x == r_cell_x - 1 && y == r_cell_y
			|| x == r_cell_x + 1 && y == r_cell_y
			|| x == r_cell_x - 1 && y == r_cell_y + 1
			|| x == r_cell_x && y == r_cell_y + 1
			|| x == r_cell_x + 1 && y == r_cell_y + 1) {
		  std::cout << "Skipped purge of " << x << "-" << y << std::endl;
		  
		} else {
		  std::string name {""};
		  //Be sure to remove the node before deleting it or it leaks memory.
		  v_terrains[x][y]->get_root().remove_node();
		  delete v_terrains[x][y];
		  v_terrains[x][y] = NULL;
		  std::cout << "Deleted Terrain" << x << "-" << y << std::endl;
		}
	  }
	}
  }
}

int TerrainManager::parse_terrains_local() {
  if (check_cfg_vars() != 0) {
	return(-1);
  }

  int max_x = ecl_to_int(LispSystem::lisp("*max-x*"));
  int max_y = ecl_to_int(LispSystem::lisp("*max-y*"));

  v_terrains.resize(max_x+1, std::vector<GeoMipTerrain*>(max_y+1, NULL));

  load_cells_nearby();

  std::cout << "v_terrains has size " << v_terrains.size() << std::endl;
  return(0);
}

void TerrainManager::build_terrains() {
  parse_terrains_local();
  
  //keep updating terrain w/ task
  p_update_terrain = new GenericAsyncTask("Updates terrain", (&update_terrain), this);
  p_update_lod = new GenericAsyncTask("Updates terrain LODs", (&update_lods), this);
  //p_update_terrain->set_task_chain("worldThreads");
  AsyncTaskManager::get_global_ptr()->add(p_update_terrain);
  AsyncTaskManager::get_global_ptr()->add(p_update_lod);

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
  traverser.traverse(M_A->window->get_render());
 
  for(int i=0; i<p_queue->get_num_entries() ; ++i) {
	CollisionEntry *entry = p_queue->get_entry(i);
	std::cout << *entry << std::endl;
  }
}

int TerrainManager::get_cell_x(int xpos) {
  return(xpos / TerrainManager::terrain_map_size);
}

int TerrainManager::get_cell_y(int ypos) {
  return(ypos / TerrainManager::terrain_map_size);
}

void TerrainManager::get_terrain_settings() {
  terrain_map_size = ecl_to_int(LispSystem::lisp("*terrain-map-size*"));
  t_brute_force = ecl_to_bool(LispSystem::lisp("*terrain-bruteforce*"));
  t_border_stitch = ecl_to_bool(LispSystem::lisp("*terrain-border-stitching*"));
  t_block_size = ecl_to_int(LispSystem::lisp("*terrain-block-size*"));
  t_near_lod = ecl_to_int(LispSystem::lisp("*terrain-near-lod*"));
  t_far_lod = ecl_to_int(LispSystem::lisp("*terrain-far-lod*"));
  t_min_q = ecl_to_int(LispSystem::lisp("*terrain-min-quality*"));
}
