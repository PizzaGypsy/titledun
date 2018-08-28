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

PNMImage* chunk_image(const PNMImage& big_i, int x, int y, int chunk_size) {
  PNMImage* output = new PNMImage(chunk_size+1, chunk_size+1);
  output->remove_alpha();
  output->set_color_type(PNMImage::ColorType::CT_grayscale);

  const int fromx = x*chunk_size;
  const int fromy = y*chunk_size;
  
  int itx, ity;
  for(ity=fromy ; ity<=fromy+chunk_size ; ity++) {
	for(itx=fromx ; itx<=fromx+chunk_size ; itx++) {
	  
	  if(ity-fromy==0 && y!=0) {
		output->set_xel_val(itx-fromx, 0, big_i.get_xel_val(itx, ity-1));
	  } else if(itx-fromx==0 && x!=0) {
		output->set_xel_val(0, ity-fromy, big_i.get_xel_val(itx-1, ity));

		/*
	  } else if(ity-fromy==1 && x!=0) {
		output->set_xel_val(itx-fromx, 1, big_i.get_xel_val(itx-2, ity));
	  } else if(itx-fromx==1 && y!=0) {
		output->set_xel_val(1, ity-fromy, big_i.get_xel_val(itx, ity-2));

	  } else if(ity-fromy==2 && x!=0) {
		output->set_xel_val(itx-fromx, 2, big_i.get_xel_val(itx-3, ity));
	  } else if(itx-fromx==2 && y!=0) {
		output->set_xel_val(2, ity-fromy, big_i.get_xel_val(itx, ity-3));

	  } else if(ity-fromy==3 && x!=0) {
		output->set_xel_val(itx-fromx, 3, big_i.get_xel_val(itx-4, ity));
	  } else if(itx-fromx==3 && y!=0) {
		output->set_xel_val(3, ity-fromy, big_i.get_xel_val(itx, ity-4));
		*/
		
	  } else {
		output->set_xel_val(itx-fromx, ity-fromy, big_i.get_xel_val(itx, ity));
	  }
	}
  }
  return(output);
}
