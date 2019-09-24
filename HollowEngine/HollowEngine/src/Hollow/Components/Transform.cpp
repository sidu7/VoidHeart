#include <hollowpch.h>
#include "Transform.h"

namespace Hollow {

	Transform Transform::instance;

	void Transform::DebugDisplay()
	{
		if (ImGui::TreeNode("Transform"))
		{
			//ImGui::SliderFloat3("Position", &mPosition[0], -5.0f, 5.0f);
			ImGui::InputFloat3("Position", &mPosition[0]);
			ImGui::TreePop();
		}
	}

}