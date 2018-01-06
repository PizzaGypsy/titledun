#pragma once

/*
  This class is the generic appstate class.
*/

#include <string>

#include <pandaFramework.h>
#include <pandaSystem.h>

class AppState;

class AppStateListener
{
public:
  AppStateListener(){};
  virtual ~AppStateListener(){};

  virtual void manage_app_state(std::string state_name, AppState* state) = 0;

  virtual AppState*	find_by_name(std::string state_name) = 0;
  virtual void		change_app_state(AppState *state) = 0;
  virtual bool		push_app_state(AppState* state) = 0;
  virtual void		pop_app_state() = 0;
  virtual void		pause_app_state() = 0;
  virtual void		shutdown() = 0;
  virtual void 		exit_app_state(AppState* state) = 0;
  virtual void      pop_all_and_push_app_state(AppState* state) = 0;
};

class AppState
{
public:
	static void create(AppStateListener* parent, const std::string name){};

	void destroy(){delete this;}

	virtual void enter() = 0;
	virtual void exit() = 0;
	virtual bool pause(){return true;}
	virtual void resume(){};
  //virtual void update(double time_since_last_frame) = 0; //use task instead

protected:
	AppState(){};

  AppState*	find_by_name(std::string state_name){return m_pParent->find_by_name(state_name);}
	void		change_app_state(AppState* state){m_pParent->change_app_state(state);}
	bool		push_app_state(AppState* state){return m_pParent->push_app_state(state);}
	void		pop_app_state(){m_pParent->pop_app_state();}
	void		shutdown(){m_pParent->shutdown();}
    void        pop_all_and_push_app_state(AppState* state){m_pParent->pop_all_and_push_app_state(state);}

	AppStateListener*			m_pParent;

};

#define DECLARE_APPSTATE_CLASS(T)										\
static void create(AppStateListener* parent, const std::string name)	\
{																		\
	T* my_app_state = new T();											\
	my_app_state->m_pParent = parent;										\
	parent->manage_app_state(name, my_app_state);							\
}
