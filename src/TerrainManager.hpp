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
  
  std::vector< std::vector <GeoMipTerrain* >> vector_o_terrains;
  std::vector< LPoint2 > positions;
  NodePath *terrain_node;

  CollisionHandlerQueue* queue;

  GeoMipTerrain find_closest_terrain();

  static const int terrain_map_size = 0x200;

private:
  NodePath terrain_objects = MainApp::get_instance()->window->get_render().attach_new_node("Terrain Node");
  static TerrainManager* p_instance;

  static inline AsyncTask::DoneStatus update_terrain(GenericAsyncTask* task, void* data);

  void enable_collisions();
  static AsyncTask::DoneStatus report_collisions(GenericAsyncTask* task, void* data);

  bool cfg_read;

  int mk_cfg(void);

  static libconfig::Config terrain_cfg;

  GeoMipTerrain* terrain;

  CollisionTraverser traverser;

};
