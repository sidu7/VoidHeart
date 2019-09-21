#pragma once

#include "Component.h"

namespace Hollow
{
	// Test Component has to be deleted afterwards
	class TestComponent : public Component
	{
		REGISTERCOMPONENT(TestComponent,"TestComponent")
	public:
		
		void Init();
		void Clear() {}
		virtual ~TestComponent() {}
		int mValue = 0;
	};
}