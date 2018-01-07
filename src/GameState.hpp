#pragma once

/*
  This class manages the game itself and sets up all the objects and logic. 
  It only does offline play.
*/

#include "MainApp.hpp"
#include "AccumulationBuffer.hpp"
#include "AppState.hpp"
#include "TerrainManager.hpp"
#include "SkyManager.hpp"
#include "Controls.hpp"
#include "PlayerCharacter.hpp"

class GameState : public AppState {
public:
  GameState();

  DECLARE_APPSTATE_CLASS(GameState)

  void enter();
  void exit();
  bool pause();
  void resume();
  static NodePath panda_actor;

  PlayerCharacter ich_bin;

  TerrainManager* p_terrains;
  SkyManager* p_sky;
  
private:
  //  PT (NodePath) panda_actor;
  //  PT (NodePath) player_node;
  static AsyncTask::DoneStatus cam_rot(GenericAsyncTask* task, void* data);
  static AsyncTask::DoneStatus move_player_noclip(GenericAsyncTask* task, void* data);
  static AsyncTask::DoneStatus move_player(GenericAsyncTask* task, void* data);
  static AsyncTask::DoneStatus terrain_collisions(GenericAsyncTask* task, void* data);

  MainApp* m_a = MainApp::get_instance();
  void make_ui_elements();

  PandaAccum* p_acc_buffer;

  NodePath camera;

  void pc_test();

  Fog *p_game_fog;

  AsyncTask *camera_rotation_task = NULL;
  AsyncTask *move_player_task = NULL;
  AsyncTask *terrain_gravity_task = NULL;

  void make_test_panda();

  void shader_attempt();
};
