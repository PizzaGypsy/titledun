#include "RocketEventListener.hpp"
#include "RocketEventManager.hpp"

rEventListener::rEventListener(const Rocket::Core::String& value) {
  this->value = value;
}

rEventListener::~rEventListener() {}

void rEventListener::ProcessEvent(Rocket::Core::Event& event) {
  rEventManager::ProcessEvent(event, value);
}

void rEventListener::OnDetach(Rocket::Core::Element* ROCKET_UNUSED_PARAMETER(element)) {
  ROCKET_UNUSED(element);
  delete this;
}
