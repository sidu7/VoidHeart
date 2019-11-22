#include <hollowpch.h>
#include "BSplineCurve.h"

namespace Hollow
{
	BSplineCurve BSplineCurve::instance;
	
	BSplineCurve::BSplineCurve()
	{
		mCurveMatrix[0] = glm::vec4(-1.0f, 3.0f, -3.0f, 1.0f);
		mCurveMatrix[1] = glm::vec4(3.0f, -6.0f, 3.0f, 0.0f);
		mCurveMatrix[2] = glm::vec4(-3.0f, 0.0f, 3.0f, 0.0f);
		mCurveMatrix[3] = glm::vec4(1.0f, 4.0f, 1.0f, 0.0f);

		mCurveMatrix /= 6.0f;

		mCurveMatrix = glm::transpose(mCurveMatrix);
	}

	glm::vec4 BSplineCurve::GetPointOnCurve(float t, glm::mat4& controlPointMatrix)
	{
		return glm::vec4(t * t * t, t * t, t, 1.0f) * instance.mCurveMatrix * controlPointMatrix;
	}

	glm::vec4 BSplineCurve::GetDerivativeOfPointOnCurve(float t, glm::mat4& controlPointMatrix)
	{
		return glm::vec4(3 * t * t, 2 * t, 1.0f, 0.0f) * instance.mCurveMatrix * controlPointMatrix;
	}
}
