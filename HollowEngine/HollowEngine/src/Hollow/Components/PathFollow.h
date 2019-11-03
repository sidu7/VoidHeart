#pragma once
#include "Component.h"

namespace Hollow
{
#define MAKE_ARCTABLE_ENTRY(x,y,z) std::make_pair(x,std::make_pair(y,z))
#define ARCTABLE_ENTRY std::pair<float, std::pair<float, int>>

	class VertexArray;
	
	class HOLLOW_API PathFollow : public Component
	{
		REGISTERCOMPONENT(PathFollow);
	public:
		void Init();
		void Clear();
		void Serialize(rapidjson::Value::Object data);
		void DebugDisplay();

	public:
		std::vector<glm::vec4> mControlPoints;
		std::vector<glm::mat4> mControlPointsMatrices;
		std::vector<ARCTABLE_ENTRY> mArcLengthTable;
		float mPathTolerance;
		float mPathRunTime;
		// Debug Path Data
		bool mDebugPath;
		VertexArray* mpCurveVAO;
		unsigned int mCurvePointsCount;
		bool mShowControlWindow;
		bool mControlPointsChanged;
	};
}
