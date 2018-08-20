#pragma once
#include <functional>
#include <vector>

#include "MainApp.hpp"

typedef std::function<void (Rocket::Core::Event&, const Rocket::Core::String&)> ProcessEventsFun;

class rEventManager {
public:
  static void Shutdown();
  static void RegisterEventHandler(const Rocket::Core::String& handler_name,
								   std::function<void (Rocket::Core::Event&, const Rocket::Core::String&)> f);
  
  static void ProcessEvent(Rocket::Core::Event& event, const Rocket::Core::String& value);
  static bool LoadWindow(const Rocket::Core::String& window_name);

private:
  std::vector<ProcessEventsFun> f_handlers;
  rEventManager();
  ~rEventManager();
};
