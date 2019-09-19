#pragma once

#include "Component.h"

namespace Hollow
{
	// Test Component has to be deleted afterwards
	class TestComponent : public Component
	{
		REGISTER(TestComponent)
	public:
		TestComponent() : Component("TestComponent", this), mValue(5) {
		}
		
		void Clear() {}
		virtual ~TestComponent() {}
		int mValue = 0;
	};

	TestComponent TestComponent::instance;
}