#pragma once

/*
  This file is for various functions that I felt should be global for their general usefulness.
*/

#include "MainApp.hpp"

int round_cell(int x);
int lpoint2_to_int(LPoint2 points);
GeoMipTerrain get_terrain_by_name(std::vector< GeoMipTerrain > terrains, std::string name);
