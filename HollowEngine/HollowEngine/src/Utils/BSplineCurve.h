#pragma once

namespace Hollow
{
	class HOLLOW_API BSplineCurve
	{
	private:
		BSplineCurve();
	public:
		static glm::vec4 GetPointOnCurve(float t, glm::mat4& controlPointMatrix);
		static glm::vec4 GetDerivativeOfPointOnCurve(float t, glm::mat4& controlPointMatrix);

	private:
		static BSplineCurve instance;
		glm::mat4 mCurveMatrix;
	};
}