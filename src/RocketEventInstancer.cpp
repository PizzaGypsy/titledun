#include "RocketEventInstancer.hpp"
#include "RocketEventListener.hpp"

rEventInstancer::rEventInstancer() {}
rEventInstancer::~rEventInstancer() {}

Rocket::Core::EventListener* rEventInstancer::InstanceEventListener(const Rocket::Core::String& value,
																	Rocket::Core::Element* element) {
  return(new rEventListener(value));
}

void rEventInstancer::Release() {
  delete this;
}
