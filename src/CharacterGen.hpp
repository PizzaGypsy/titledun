#pragma once

/*
  Work in progress appstate for character generation.
*/

#include "MainApp.hpp"

class CharacterGen : public AppState {
public:

  DECLARE_APPSTATE_CLASS(CharacterGen)
  
  NodePath player_char;
  Texture* tex1;
  
  CharacterGen();
  ~CharacterGen();

  void enter();
  void exit();
  bool pause();
  void resume();

  std::vector<NodePath> all_nodes;
  
private:
  void setup_background();
  void make_sliders();
  void make_buttons();
  void make_text();
  int load_markings();
  
  NodePath camera;
  MainApp* m_a = MainApp::get_instance();

  PT(TextNode) p_layer_zero = NULL;
  PT(TextNode) p_layer_one = NULL;
  NodePath layer_zero_np;
  NodePath layer_one_np;

  static void previous_layer(const Event* e, void* data);
  static void next_layer(const Event* e, void* data);

  void load_species_list();
  void initialise_data_structures();

  int num_layers = 0;
  int num_marks = 0;
  
  int layer_idx = 0;
  int mark_idx = 0;
  
  std::vector<std::string> sp_list;
  std::vector<TextureStage*> v_layers;
  std::vector<Texture*> v_markings;
  std::vector<TextNode*> v_text_layers;
  TextureStage* layer0 = TextureStage::get_default();
  TextureStage* layer1 = NULL;

  void set_model();

  CardMaker* fullscreen_card = NULL;
  NodePath* bg_np = NULL;

  static void apply_color(const Event* e, void* data);

  void make_ui_elements();

  NodePath exit_np;
  PT(PGButton) exit_button = NULL;

  NodePath previous_np;
  PT(PGButton) previous_button = NULL;

  NodePath next_np;
  PT(PGButton) next_button = NULL;

  NodePath apply_np;
  PT(PGButton) apply_button = NULL;

  NodePath slider1_np;
  PT(PGSliderBar) slider1 = NULL;
  NodePath slider2_np;
  PT(PGSliderBar) slider2 = NULL;
  NodePath slider3_np;
  PT(PGSliderBar) slider3 = NULL;

  static AsyncTask::DoneStatus update_sliders(GenericAsyncTask* task, void* data);
  GenericAsyncTask* update_slider_task = NULL;
};
