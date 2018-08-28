#include <iostream>
#include "TerrainCompiler.hpp"
#include "UtilFunctions.hpp"

bool TerrainCompiler::compile_terrain(GeoMipTerrain* terrain, std::string& output) {
  //wrong documentation?
  //return(terrain->get_root().encode_to_bam_stream(output));
  return(false);
}

bool TerrainCompiler::generate_heightmaps(const PNMImage& maps, int chunk_size) {
  //check to see if our chunks fit. avoid the edges if not.
  bool not_even = false;
  
  if(fmod(maps.get_read_x_size(), chunk_size)!=0) {
	std::cout<<"TerrainCompiler::generate_heightmaps(PNMImage&<"<<maps.get_read_x_size()<<", "<<maps.get_read_y_size()
			 <<">) got a terrain map not divisible by "
			 <<chunk_size<<"!\nEdges will be truncated..."<<std::endl;
	not_even = true;
  } else if(fmod(maps.get_read_y_size(), chunk_size)!=0) {
	std::cout<<"TerrainCompiler::generate_heightmaps(PNMImage&<"<<maps.get_read_x_size()<<", "<<maps.get_read_y_size()
			 <<">) got a terrain map not divisible by "
			 <<chunk_size<<"!\nEdges will be truncated..."<<std::endl;
	not_even = true;
  }
  
  //How many maps we've got in the larger map.
  int mapsx = maps.get_read_x_size() / chunk_size - not_even;
  int mapsy = maps.get_read_y_size() / chunk_size - not_even;
  
  int itx, ity;
  
  for(ity=0 ; ity<mapsy ; ity++) {
	for(itx=0 ; itx<mapsx ; itx++) {
	  PNMImage* temp_img = chunk_image(maps, itx, ity, chunk_size);
	  
	  std::string path = build_terrain_path("output/t", itx, ity);
	  temp_img->write(Filename(path));
	  
	  std::cout<<"wrote: "<<path<<std::endl;
	  
	  delete temp_img;
	}
  }
  
  return(not_even);
}

std::string TerrainCompiler::build_terrain_path(const std::string& folder, int x, int y) {
  std::string path = folder;
  path += std::to_string(x);
  path += "_";
  path += std::to_string(y);
  path += ".png";

  return(path);
}
