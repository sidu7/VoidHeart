#pragma once

#include "Component.h"

// Test Component has to be deleted afterwards
class TestComponent : public Component
{
public:
	TestComponent() : mValue(5) {}
	virtual ~TestComponent() {}
	int mValue;
};
