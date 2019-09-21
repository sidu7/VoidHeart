#include "PrototypeComponent.h"

PrototypeComponent PrototypeComponent::instance;

PrototypeComponent::PrototypeComponent(): Hollow::Component("PrototypeComponent",this)
{
}

PrototypeComponent::~PrototypeComponent()
{
}

void PrototypeComponent::Clear()
{
}
