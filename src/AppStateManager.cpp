
#include "AppStateManager.hpp"

AppStateManager::AppStateManager()
{
	b_shutdown = false;
}


AppStateManager::~AppStateManager()
{
	state_info si;

    while(!active_state_stack.empty())
	{
		active_state_stack.back()->exit();
		active_state_stack.pop_back();
	}

	while(!states.empty())
	{
		si = states.back();
        si.state->destroy();
        states.pop_back();
	}
}


void AppStateManager::manage_app_state(std::string state_name, AppState* state)
{
	try
	{
		state_info new_state_info;
		new_state_info.name = state_name;
		new_state_info.state = state;
		states.push_back(new_state_info);
	}
	catch(std::exception& e)
	{
		delete state;
		//throw
	}
}


AppState* AppStateManager::find_by_name(std::string state_name)
{
	std::vector<state_info>::iterator itr;

	for(itr=states.begin() ; itr!=states.end() ; itr++)
	{
		if(itr->name==state_name)
			return itr->state;
	}

	return 0;
}


void AppStateManager::start(AppState* state)
{
  change_app_state(state);
}


void AppStateManager::change_app_state(AppState* state)
{
	if(!active_state_stack.empty())
	{
		active_state_stack.back()->exit();
		active_state_stack.pop_back();
	}

	active_state_stack.push_back(state);
	init(state);
	active_state_stack.back()->enter();
}

void AppStateManager::exit_app_state(AppState* state) {
  if (!active_state_stack.empty()) {
	active_state_stack.back()->exit();
  }
}


bool AppStateManager::push_app_state(AppState* state)
{
	if(!active_state_stack.empty())
	{
		if(!active_state_stack.back()->pause())
			return false;
	}

	active_state_stack.push_back(state);
	init(state);
	active_state_stack.back()->enter();

	return true;
}


void AppStateManager::pop_app_state()
{
	if(!active_state_stack.empty())
	{
		active_state_stack.back()->exit();
		active_state_stack.pop_back();
	}

	if(!active_state_stack.empty())
	{
		init(active_state_stack.back());
		active_state_stack.back()->resume();
	}
    else
		shutdown();
}


void AppStateManager::pop_all_and_push_app_state(AppState* state)
{
    while(!active_state_stack.empty())
    {
        active_state_stack.back()->exit();
        active_state_stack.pop_back();
    }

    push_app_state(state);
}


void AppStateManager::pause_app_state()
{
	if(!active_state_stack.empty())
	{
		active_state_stack.back()->pause();
	}

	if(active_state_stack.size() > 2)
	{
		init(active_state_stack.at(active_state_stack.size() - 2));
		active_state_stack.at(active_state_stack.size() - 2)->resume();
	}
}


void AppStateManager::shutdown()
{
	b_shutdown = true;
}


void AppStateManager::init(AppState* state)
{
}

