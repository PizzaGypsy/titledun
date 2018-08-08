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
#include "LispAPI.hpp"

class GameState : public AppState {
public:
  GameState();

  DECLARE_APPSTATE_CLASS(GameState)

  void enter();
  void exit();
  bool pause();
  void resume();
  static NodePath panda_actor;

  PlayerCharacter* pc_obj = NULL;

  TerrainManager* p_terrains = NULL;
  SkyManager* p_sky = NULL;
  
private:
  //  PT (NodePath) panda_actor;
  //  PT (NodePath) player_node;
  static AsyncTask::DoneStatus cam_rot(GenericAsyncTask* task, void* data);
  static AsyncTask::DoneStatus move_player_noclip(GenericAsyncTask* task, void* data);
  static AsyncTask::DoneStatus move_player(GenericAsyncTask* task, void* data);
  static AsyncTask::DoneStatus terrain_collisions(GenericAsyncTask* task, void* data);

  void make_ui_elements();

  PandaAccum* p_acc_buffer = NULL;

  NodePath camera;

  void pc_test();

  AsyncTask *camera_rotation_task = NULL;
  AsyncTask *move_player_task = NULL;
  AsyncTask *terrain_gravity_task = NULL;

  void make_test_panda();

  void shader_attempt();
};
