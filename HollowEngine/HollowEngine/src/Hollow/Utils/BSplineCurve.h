#pragma once
#include "Hollow/Core/Core.h"

namespace Hollow
{
	class BSplineCurve
	{
	private:
		BSplineCurve();
	public:
		HOLLOW_API  static glm::vec4 GetPointOnCurve(float t, glm::mat4& controlPointMatrix);
		HOLLOW_API  static glm::vec4 GetDerivativeOfPointOnCurve(float t, glm::mat4& controlPointMatrix);

	private:
		static BSplineCurve instance;
		glm::mat4 mCurveMatrix;
	};
}