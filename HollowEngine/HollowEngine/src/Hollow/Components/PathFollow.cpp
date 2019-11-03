#include <hollowpch.h>
#include "PathFollow.h"

#include "Hollow/Managers/RenderManager.h"

#include "Hollow/Graphics/VertexArray.h"

namespace Hollow
{
	PathFollow PathFollow::instance;

	void PathFollow::Init()
	{
		mPathTolerance = 0.0001f;
		mPathRunTime = 0.0f;
	}

	void PathFollow::Clear()
	{
		delete mpCurveVAO;
		mControlPoints.clear();
		mControlPointsMatrices.clear();
		mArcLengthTable.clear();
	}

	void PathFollow::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("ControlPoints"))
		{
			const auto points = data["ControlPoints"].GetArray();
			mControlPoints.clear();
			mControlPoints.reserve(points.Size());
			for (unsigned int i = 0; i < points.Size(); ++i)
			{
				auto arr = points[i].GetArray();
				mControlPoints.emplace_back(glm::vec4(JSONHelper::GetVec3F(points[i].GetArray()), 1.0f));
			}
		}
	}

	void PathFollow::DebugDisplay()
	{
		if (ImGui::TreeNode("PathFollow"))
		{
			ImGui::Checkbox("Show Path", &mDebugPath);
			if(mDebugPath)
			{
				DebugPathData pathData;
				pathData.mCurveVAO = mpCurveVAO;
				pathData.mCurvePointsCount = mCurvePointsCount;
				RenderManager::Instance().mDebugPathData.push_back(pathData);
			}
			ImGui::TreePop();
		}
	}
}