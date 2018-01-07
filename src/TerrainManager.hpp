#pragma once

/*
  This class parses a config and loads the tiling terrains into a 2D std::vector.
  All collision functions here are deprecated.
*/

#include "MainApp.hpp"

class TerrainManager {
public:
  TerrainManager();
  ~TerrainManager();

  void build_terrains();
  int parse_terrains();
  void set_defaults(GeoMipTerrain* terrain, const std::string name, const std::string texture, int x, int y);

  void destroy_terrains();
  void hide_terrains();
  
  std::vector< std::vector <GeoMipTerrain* >> v_terrains;
  //std::vector< LPoint2 > positions;
  NodePath *p_terrain_node;

  CollisionHandlerQueue* p_queue;

  GeoMipTerrain find_closest_terrain();

  static const int terrain_map_size = 0x200;

private:
  NodePath terrain_objects_np = MainApp::get_instance()->window->get_render().attach_new_node("Terrain Node");
  static TerrainManager* p_instance;

  static inline AsyncTask::DoneStatus update_terrain(GenericAsyncTask* task, void* data);

  void enable_collisions();

  bool cfg_read;

  int mk_cfg(void);

  static libconfig::Config terrain_cfg;

  GeoMipTerrain* p_terrain;

  CollisionTraverser traverser;

};
