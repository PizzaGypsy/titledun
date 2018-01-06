#include "TerrainManager.hpp"

libconfig::Config TerrainManager::terrain_cfg;

inline AsyncTask::DoneStatus TerrainManager::update_terrain(GenericAsyncTask* task, void* data) {
  //iterate over vector of terrains and update LODs dynamically based on the camera's position.
  return AsyncTask::DS_cont;
}


TerrainManager::TerrainManager() {
  terrain_node = &terrain_objects;
  cfg_read = false;
}

TerrainManager::~TerrainManager() {
  AsyncTaskManager::get_global_ptr()->remove(AsyncTaskManager::get_global_ptr()->find_task("Updates terrain"));
  terrain_node->remove_node();
  
  for (int x = 0 ; x < vector_o_terrains.size() ; x++) {
	for (int y = 0 ; y < vector_o_terrains.size() ; y++) {
	  //vector_o_terrains[x][y]->get_root().remove_node();
	  delete vector_o_terrains[x][y];
	}
  }
  vector_o_terrains.clear();
}

void TerrainManager::destroy_terrains() {
  AsyncTaskManager::get_global_ptr()->remove(AsyncTaskManager::get_global_ptr()->find_task("Updates terrain"));
  for (int it = 0 ; it < vector_o_terrains.size() ; it++) {
	//vector_o_terrains[it].get_root().clear();
	//vector_o_terrains[it]->get_root().remove_node();
  }
  //vector_o_terrains.clear();
}

int TerrainManager::mk_cfg(void) {
  libconfig::Setting &root = MainApp::cfg_parser.getRoot();
  libconfig::Setting &tvar = root.add("terrains", libconfig::Setting::TypeGroup);

  try {
	MainApp::cfg_parser.writeFile("terrains.cfg");
	std::cerr << "New configuration successfully written to: " << "terrains.cfg" << std::endl;
	
  } catch (const libconfig::FileIOException &fioex) {
	  std::cerr << "I/O error while writing file: " << "terrains.cfg" << std::endl;
	  return(1);
  }
  return(0);
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
  terrain->get_root().reparent_to(*(terrain_node));
  terrain->get_root().set_sz(50);
  terrain->get_root().set_pos((x * 0x200), (y * 0x200), 0);
  terrain->set_min_level(2);
  terrain->generate();
}

int TerrainManager::parse_terrains() {
  try {
	MainApp::cfg_parser.readFile("terrains.cfg");
	
  } catch (const libconfig::FileIOException &fioex) {
	std::cerr << "I/O error while reading file." << std::endl;
	return(1);
	
  } catch (const libconfig::ParseException &pex) {
	std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
			  << " - " << pex.getError() << std::endl;
	return(2);
  }
  
  libconfig::Setting &root = MainApp::cfg_parser.getRoot();

  int count = 0;
  int max_x = 0;
  int max_y = 0;

  try {
	const libconfig::Setting &c_terrains = root["terrains"]["list"];
	count = c_terrains.getLength();
	root["globals"].lookupValue("max_x", max_x);
	root["globals"].lookupValue("max_y", max_y);

	std::cout << "max_x: " << max_x << std::endl << "max_y: " << max_y << std::endl;
	
	vector_o_terrains.resize(max_x + 1, std::vector<GeoMipTerrain*>(max_y + 1, NULL));

	for (int i = 0 ; i < count ; i++) {
	  const libconfig::Setting &c_terrain = c_terrains[i];

	  std::string name;
	  std::string texture1;
	  int x;
	  int y;

	  if (!(c_terrain.lookupValue("name", name)
			&& c_terrain.lookupValue("texture1", texture1)
			&& c_terrain.lookupValue("x", x)
			&& c_terrain.lookupValue("y", y)
			))
		continue;

	  terrain = new GeoMipTerrain(name);
	  set_defaults(terrain, name, texture1, x, y);

	  //add terrains to vector, so I can iterate over all instances as needed
	  //vector_o_terrains.push_back(*(terrain));
	  try {
		vector_o_terrains.at(x).at(y) = terrain;
	  } catch (const std::out_of_range& oor) {
		std::cerr << "Out of range error: " << oor.what() << std::endl;
	  }

	  std::cout << "Terrain loaded: " << count <<
		"@" << x << "&" << y << std::endl;
	}
  } catch (const libconfig::SettingNotFoundException &nfex) {
	//ignore
  }
  std::cout << "vector_o_terrains has size " << vector_o_terrains.size() << std::endl;
  
  cfg_read = true;

  return(0);
}

void TerrainManager::build_terrains() {
  if (cfg_read == false) {
	parse_terrains();
  }
  //keep updating terrain w/ task
  //AsyncTaskManager::get_global_ptr()->add(new GenericAsyncTask("Updates terrain", (&update_terrain), (void*) NULL));

  //enable_collisions();
  //AsyncTaskManager::get_global_ptr()->add(new GenericAsyncTask("Reports terrain collisions", (&report_collisions), (void*) NULL));
  
  terrain_node->show();
}

void TerrainManager::hide_terrains() {
  AsyncTaskManager::get_global_ptr()->remove(AsyncTaskManager::get_global_ptr()->find_task("Updates terrain"));
  terrain_node->hide();
}

void TerrainManager::enable_collisions() {
  BitMask32 good_mask = BitMask32(0x0);
  terrain_node->set_collide_mask(good_mask);
  
  queue = new CollisionHandlerQueue();
  traverser.add_collider(*(terrain_node), queue);
  traverser.traverse(MainApp::get_instance()->window->get_render());
 
  for(int i = 0; i < queue->get_num_entries(); ++i) {
	CollisionEntry *entry = queue->get_entry(i);
	std::cout << *entry << std::endl;
  }
}

AsyncTask::DoneStatus TerrainManager::report_collisions(GenericAsyncTask* task, void* data) {
  /*
  for(int i = 0; i < TerrainManager::get_instance()->queue->get_num_entries(); ++i) {
	CollisionEntry *entry = TerrainManager::get_instance()->queue->get_entry(i);
	std::cout << *entry << std::endl;
  }
  */
}
