#include "UtilFunctions.hpp"

int round_cell(int x) {
  if (x < 0) {
	return(floor(x));
  } else {
	return(ceil(x));
  }
}

int lpoint2_to_int(LPoint2 points) {
  return(abs(points.get_x()) + abs(points.get_y()));
}

GeoMipTerrain get_terrain_by_name(std::vector< GeoMipTerrain > terrains, std::string name) {
  for (int it = 0 ; it < terrains.size() ; it++) {
	if (name == terrains[it].get_root().get_name()) {
	  return (terrains[it]);
	}
  }
  std::cout << "Terrain " << name << "not found!" << std::endl;
  return (terrains[0]);
}

void texture_fill_color(Texture* tex, float red, float green, float blue) {
  PNMImage temp_image = PNMImage(256, 256);
  
  temp_image.add_alpha();
  
  tex->store(temp_image);
  temp_image.fill(red, green, blue);

  tex->load(temp_image);
}

void texture_gauss_filter(Texture* tex) {
  PNMImage temp_image = PNMImage(tex->get_x_size(), tex->get_y_size());

  tex->store(temp_image);
  temp_image.gaussian_filter(0.5);
  tex->load(temp_image);
}
