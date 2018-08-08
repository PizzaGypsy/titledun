#pragma once

/*
  This class parses a config and loads the tiling terrains into a 2D std::vector.
  All collision functions here are deprecated.
*/

#include "MainApp.hpp"
#include "LispSystem.hpp"

class TerrainManager {
public:
  TerrainManager();
  ~TerrainManager();

  void build_terrains();
  int parse_terrains_local();
  static void set_defaults(GeoMipTerrain* terrain, const std::string name, const std::string texture, int x, int y);

  void hide_terrains();
  
  std::vector< std::vector <GeoMipTerrain* >> v_terrains;
  NodePath *p_terrain_node;

  CollisionHandlerQueue* p_queue;

  int load_cells_nearby();
  int load_cell(int cell_x, int cell_y);

  static const int terrain_map_size = 0x400; //0x200
  static int r_cell_x;
  static int r_cell_y;

  static int p_cell_x;
  static int p_cell_y;

  void purge_cell_buffer();
  
  static int get_cell_x(int xpos);
  static int get_cell_y(int ypos);

  AsyncTaskChain* p_b_world_threads = NULL;

private:
  NodePath terrain_objects_np = MainApp::get_instance()->window->get_render().attach_new_node("Terrain Node");
  static TerrainManager* p_instance;

  static inline AsyncTask::DoneStatus update_terrain(GenericAsyncTask* task, void* data);
  static AsyncTask::DoneStatus set_defaults_async(GenericAsyncTask* task, void* data);
  static void cleanup_loader(GenericAsyncTask* task, bool b, void* data);

  void enable_collisions();

  GeoMipTerrain* p_terrain;

  CollisionTraverser traverser;

  PT(AsyncTask) p_update_terrain = NULL;

  int check_cfg_vars();
};

class t_container {
public:
  t_container(GeoMipTerrain* t, const std::string n, const std::string tex1, int c_x, int c_y) {
	p_terrain = t;
	name = n;
	texture1 = tex1;
	cell_x = c_x;
	cell_y = c_y;
  }
  GeoMipTerrain* p_terrain = NULL;
  std::string name = {""};
  std::string texture1 = {""};
  int cell_x {0};
  int cell_y {0};
};
