#pragma once

#include "Component.h"

class TestComponent : public Component
{
public:
	TestComponent() : mValue(5) {}
	virtual ~TestComponent() {}
	int mValue;
};