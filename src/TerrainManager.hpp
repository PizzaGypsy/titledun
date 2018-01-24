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
  int parse_terrains();
  int parse_terrains_lisp();
  void set_defaults(GeoMipTerrain* terrain, const std::string name, const std::string texture, int x, int y);

  void destroy_terrains();
  void hide_terrains();
  
  std::vector< std::vector <GeoMipTerrain* >> v_terrains;
  NodePath *p_terrain_node;

  CollisionHandlerQueue* p_queue;

  GeoMipTerrain find_closest_terrain();

  static const int terrain_map_size = 0x800; //0x200

private:
  NodePath terrain_objects_np = MainApp::get_instance()->window->get_render().attach_new_node("Terrain Node");
  static TerrainManager* p_instance;

  static inline AsyncTask::DoneStatus update_terrain(GenericAsyncTask* task, void* data);

  void enable_collisions();

  GeoMipTerrain* p_terrain;

  CollisionTraverser traverser;

  int check_cfg_vars();
};
