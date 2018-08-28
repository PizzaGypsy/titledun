#pragma once
#include <nodePath.h>
#include <geoMipTerrain.h>
#include <filename.h>
#include <pnmImage.h>

class TerrainCompiler {
public:
  static bool compile_terrain(GeoMipTerrain* terrain, std::string& output);
  static bool generate_heightmaps(const PNMImage& maps, int chunk_size);
  static std::string build_terrain_path(const std::string& folder, int x, int y);
};
