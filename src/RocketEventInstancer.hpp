#pragma once

#include <Rocket/Core.h>
#include <Rocket/Core/String.h>

class rEventInstancer : public Rocket::Core::EventListenerInstancer {
public:
  rEventInstancer();
  virtual ~rEventInstancer();

  virtual Rocket::Core::EventListener* InstanceEventListener(const Rocket::Core::String& value,
															 Rocket::Core::Element* element);

  virtual void Release();
};
