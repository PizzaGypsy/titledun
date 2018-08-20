#pragma once

#include "MainApp.hpp"

class rEventListener : public Rocket::Core::EventListener {
public:
  rEventListener(const Rocket::Core::String& value);
  virtual ~rEventListener();

  virtual void ProcessEvent(Rocket::Core::Event& event);
  virtual void OnDetach(Rocket::Core::Element* element);

private:
  Rocket::Core::String value;
};
