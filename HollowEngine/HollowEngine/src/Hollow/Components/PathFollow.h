#pragma once
#include "Component.h"

namespace Hollow
{
#define MAKE_ARCTABLE_ENTRY(x,y,z) std::make_pair(x,std::make_pair(y,z))
#define ARCTABLE_ENTRY std::pair<float, std::pair<float, int>>

	class VertexArray;
	
	class PathFollow : public Component
	{
		REGISTERCOMPONENT(PathFollow);
	public:
		/// <summary>
		/// Initializes the PathFollow component.
		/// </summary>
		HOLLOW_API void Init() override;

		/// <summary>
		/// Clears the PathFollow component.
		/// </summary>
		HOLLOW_API void Clear() override;

		/// <summary>
		/// Serializes the specified data of the PathFollow component from the JSON file.
		/// </summary>
		/// <param name="data">JSON object.</param>
		HOLLOW_API void Serialize(rapidjson::Value::Object data) override;

		/// <summary>
		/// DeSerialize the current PathFollow component into a JSON file.
		/// </summary>
		/// <param name="writer">The writer of the JSON file.</param>
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;

		/// <summary>
		/// To set the Debug Display for the PathFollow Component.
		/// </summary>
		HOLLOW_API void DebugDisplay() override;

	public:
		std::vector<glm::vec4> mControlPoints;
		std::vector<glm::mat4> mControlPointsMatrices;
		std::vector<ARCTABLE_ENTRY> mArcLengthTable;
		float mPathTolerance;
		float mPathRunTime;
		bool mMove;
		
		// Debug Path Data
		bool mDebugPath;
		VertexArray* mpCurveVAO;
		unsigned int mCurvePointsCount;
		bool mShowControlWindow;
		bool mControlPointsChanged;
	};
}
