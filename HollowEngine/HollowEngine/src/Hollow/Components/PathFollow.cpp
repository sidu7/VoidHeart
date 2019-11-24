#include <hollowpch.h>
#include "PathFollow.h"
#include "Hollow/Graphics/VertexArray.h"

namespace Hollow
{
	PathFollow PathFollow::instance;

	void PathFollow::Init()
	{
		mPathTolerance = 0.0f;
		mPathRunTime = 0.0f;
		mMove = false;

		mDebugPath = false;
		mCurvePointsCount = 0;
		mShowControlWindow = false;
		mControlPointsChanged = false;
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
		if(data.HasMember("Move"))
		{
			mMove = data["Move"].GetBool();
		}
		if(data.HasMember("Tolerance"))
		{
			mPathTolerance = data["Tolerance"].GetFloat();
		}
	}

	void PathFollow::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		writer.Key("ControlPoints");
		writer.StartArray();
		for(int i = 0; i < mControlPoints.size(); ++i)
		{
			writer.StartArray();
			writer.Double(mControlPoints[i].x);
			writer.Double(mControlPoints[i].y);
			writer.Double(mControlPoints[i].z);
			writer.EndArray();
		}
		writer.EndArray();
		JSONHelper::Write("Move", mMove, writer);
		JSONHelper::Write("Tolerance", mPathTolerance, writer);
	}

	void PathFollow::DebugDisplay()
	{
		ImGui::Checkbox("Show Path", &mDebugPath);
		ImGui::Checkbox("Move", &mMove);
		if (ImGui::Button(mShowControlWindow ? "Hide Control Points Window" : "Show Control Points Window"))
		{
			mShowControlWindow = !mShowControlWindow;
		}
		if (mShowControlWindow)
		{
			ImGui::Begin("Control Points");
			if (ImGui::Button("New Control Point"))
			{
				mControlPoints.emplace_back(mControlPoints[mControlPoints.size() - 1]);
				mControlPointsChanged = true;
			}
			ImGui::SameLine();
			if(ImGui::Button("Copy Control Points To Clipboard"))
			{
				std::string control_data = "";
				for(unsigned int i = 0; i < mControlPoints.size(); ++i)
				{
					const glm::vec4& point = mControlPoints[i];
					control_data += '[' + std::to_string(point.x) + ',' + std::to_string(point.y) + ',' + std::to_string(point.z) + "],";
				}
				ImGui::SetClipboardText(control_data.c_str());
			}				
			for (unsigned int i = 0; i < mControlPoints.size(); ++i)
			{
				if (ImGui::SliderFloat3(std::string("Point" + std::to_string(i + 1)).c_str(), (float*)&mControlPoints[i], -150.0f, 150.0f, "%.1f"))
				{
					mControlPointsChanged = true;
				}
			}
			ImGui::End();
		}
	}
}