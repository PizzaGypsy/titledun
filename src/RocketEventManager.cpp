#include "RocketEventManager.hpp"
#include <map>

static ProcessEventsFun event_handler = NULL;

typedef std::map< Rocket::Core::String, ProcessEventsFun > EventHandlerMap;
EventHandlerMap event_handlers;

rEventManager::rEventManager() {}
rEventManager::~rEventManager() {}

void rEventManager::Shutdown() {
  for (EventHandlerMap::iterator i=event_handlers.begin() ; i!=event_handlers.end() ; i++) {
	(*i).second.~function();
  }
  event_handlers.clear();
  event_handler = NULL;
}

void rEventManager::RegisterEventHandler(const Rocket::Core::String& handler_name,
										 std::function<void (Rocket::Core::Event&, const Rocket::Core::String&)> handler) {
  
  EventHandlerMap::iterator it = event_handlers.find(handler_name);
  if (it!=event_handlers.end()) {
	(*it).second.~function();
	(*it).second = NULL;
  }
  event_handlers[handler_name] = handler;
}

void rEventManager::UnregisterEventHandler(const Rocket::Core::String& handler_name) {
  EventHandlerMap::iterator it = event_handlers.find(handler_name);
  if (it!=event_handlers.end()) {
	(*it).second.~function();
	(*it).second = NULL;
  }
}

void rEventManager::ProcessEvent(Rocket::Core::Event& event, const Rocket::Core::String& value) {
  Rocket::Core::StringList commands;
  Rocket::Core::StringUtilities::ExpandString(commands, value, ';');

  for (size_t i=0 ; i<commands.size() ; i++) {
	Rocket::Core::StringList values;
	Rocket::Core::StringUtilities::ExpandString(values, commands[i], ' ');

	if (values.empty()) {return;}
	
	if(values[0] == "close") {
	  Rocket::Core::ElementDocument* target_document = NULL;

	  if (values.size()>1)
		target_document = M_A->r_context->GetDocument(values[1].CString());
	  else
		target_document = event.GetTargetElement()->GetOwnerDocument();
	  
	  if (target_document != NULL) {
		UnregisterEventHandler(target_document->GetTagName());
		target_document->Close();
	  }
	  
	} else if (values[0] == "hide") {
	  Rocket::Core::ElementDocument* target_document = NULL;
	  target_document = event.GetTargetElement()->GetOwnerDocument();
	  
	  if (target_document != NULL) {
		target_document->Hide();
	  }
	} else {
	  if(event_handler!=NULL) {
		event_handler(event, commands[i]);
	  }
	}
  }
}

Rocket::Core::ElementDocument* rEventManager::LoadWindow(const Rocket::Core::String& window_name) {
  ProcessEventsFun old_event_handler = event_handler;
  EventHandlerMap::iterator iterator = event_handlers.find(window_name);
  if (iterator != event_handlers.end()) {
	event_handler = (*iterator).second;
  }
  else {
	event_handler = NULL;
  }

  Rocket::Core::String document_path = Rocket::Core::String("../cfg/ui/") + window_name + Rocket::Core::String(".rml");	
  Rocket::Core::ElementDocument* document = M_A->r_context->LoadDocument(document_path.CString());
  if (document == NULL) {
	event_handler = old_event_handler;
	return(NULL);
  }

  Rocket::Core::Element* title = document->GetElementById("title");
  if (title != NULL) {
	title->SetInnerRML(document->GetTitle());
  }

  document->Focus();
  document->Show();

  document->RemoveReference();

  return(document);
}