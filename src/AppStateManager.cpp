
#include "AppStateManager.hpp"

AppStateManager::AppStateManager()
{
	m_bShutdown = false;
}


AppStateManager::~AppStateManager()
{
	state_info si;

    while(!m_ActiveStateStack.empty())
	{
		m_ActiveStateStack.back()->exit();
		m_ActiveStateStack.pop_back();
	}

	while(!m_States.empty())
	{
		si = m_States.back();
        si.state->destroy();
        m_States.pop_back();
	}
}


void AppStateManager::manage_app_state(std::string state_name, AppState* state)
{
	try
	{
		state_info new_state_info;
		new_state_info.name = state_name;
		new_state_info.state = state;
		m_States.push_back(new_state_info);
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

	for(itr=m_States.begin();itr!=m_States.end();itr++)
	{
		if(itr->name==state_name)
			return itr->state;
	}

	return 0;
}


void AppStateManager::start(AppState* state)
{
  change_app_state(state);

  //int timeSinceLastFrame = 1;
  //int startTime = 0;

  /*while(!m_bShutdown)
  {
	if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isClosed())m_bShutdown = true;

	//Ogre::WindowEventUtilities::messagePump();

  }

  OgreFramework::getSingletonPtr()->m_pLog->logMessage("Main loop quit");*/
}


void AppStateManager::change_app_state(AppState* state)
{
	if(!m_ActiveStateStack.empty())
	{
		m_ActiveStateStack.back()->exit();
		m_ActiveStateStack.pop_back();
	}

	m_ActiveStateStack.push_back(state);
	init(state);
	m_ActiveStateStack.back()->enter();
}

void AppStateManager::exit_app_state(AppState* state) {
  if (!m_ActiveStateStack.empty()) {
	m_ActiveStateStack.back()->exit();
  }
}


bool AppStateManager::push_app_state(AppState* state)
{
	if(!m_ActiveStateStack.empty())
	{
		if(!m_ActiveStateStack.back()->pause())
			return false;
	}

	m_ActiveStateStack.push_back(state);
	init(state);
	m_ActiveStateStack.back()->enter();

	return true;
}


void AppStateManager::pop_app_state()
{
	if(!m_ActiveStateStack.empty())
	{
		m_ActiveStateStack.back()->exit();
		m_ActiveStateStack.pop_back();
	}

	if(!m_ActiveStateStack.empty())
	{
		init(m_ActiveStateStack.back());
		m_ActiveStateStack.back()->resume();
	}
    else
		shutdown();
}


void AppStateManager::pop_all_and_push_app_state(AppState* state)
{
    while(!m_ActiveStateStack.empty())
    {
        m_ActiveStateStack.back()->exit();
        m_ActiveStateStack.pop_back();
    }

    push_app_state(state);
}


void AppStateManager::pause_app_state()
{
	if(!m_ActiveStateStack.empty())
	{
		m_ActiveStateStack.back()->pause();
	}

	if(m_ActiveStateStack.size() > 2)
	{
		init(m_ActiveStateStack.at(m_ActiveStateStack.size() - 2));
		m_ActiveStateStack.at(m_ActiveStateStack.size() - 2)->resume();
	}
}


void AppStateManager::shutdown()
{
	m_bShutdown = true;
}


void AppStateManager::init(AppState* state)
{
}

