#pragma once

/*
  This class manages appstate and is created in MainApp
*/

#include <vector>
#include "AppState.hpp"

class AppStateManager : public AppStateListener
{
public:
  typedef struct
  {
	std::string name;
	AppState* state;
  } state_info;

  AppStateManager();
  ~AppStateManager();

  void manage_app_state(std::string state_name, AppState* state);

  AppState* find_by_name(std::string state_name);


  void start(AppState* state);
  void change_app_state(AppState* state);
  bool push_app_state(AppState* state);
  void pop_app_state();
  void pause_app_state();
  void shutdown();
  void exit_app_state(AppState* state);
  void pop_all_and_push_app_state(AppState* state);

protected:
	void init(AppState *state);

	std::vector<AppState*>		m_ActiveStateStack;
	std::vector<state_info>		m_States;
	bool						m_bShutdown;
};

