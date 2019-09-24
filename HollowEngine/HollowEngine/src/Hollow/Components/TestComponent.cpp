#include <hollowpch.h>
#include "TestComponent.h"

namespace Hollow
{
	TestComponent TestComponent::instance;

	void TestComponent::Init()
	{

	}
	void TestComponent::DebugDisplay()
	{
		if (ImGui::TreeNode("Test"))
		{
			ImGui::InputInt("Value", &mValue);
			ImGui::TreePop();
		}
	}
}