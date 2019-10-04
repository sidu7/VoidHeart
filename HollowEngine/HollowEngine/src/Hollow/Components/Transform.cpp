#include <hollowpch.h>
#include "Transform.h"

namespace Hollow {

	Transform Transform::instance;

	void Transform::DebugDisplay()
	{
		if (ImGui::TreeNode("Transform"))
		{
			ImGui::InputFloat3("Position", &mPosition[0]);
			// TODO: Find a better way to update transform matrix
			glm::mat4 mat(1.0f);
			mat = glm::translate(mat, mPosition);
			SetTranformationMatrix(mat);
			ImGui::TreePop();
		}
	}

	void Transform::Serialize(rapidjson::Value::Object data)
	{
		mTransformationMatrix = glm::mat4(1.0f);
	}

}