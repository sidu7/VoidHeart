#include <hollowpch.h>
#include "PathFollowSystem.h"

#include "Hollow/Components/PathFollow.h"
#include "Hollow/Components/Body.h"

#include "Hollow/Managers/FrameRateController.h"

#include "Hollow/Graphics/VertexArray.h"
#include "Hollow/Graphics/VertexBuffer.h"

#include "Utils/BSplineCurve.h"

namespace Hollow
{
	PathFollowSystem PathFollowSystem::instance;
	
	void PathFollowSystem::AddGameObject(GameObject* pGameObject)
	{
		if(CheckComponents<PathFollow,Body>(pGameObject))
		{
			PathFollow* pathFollow = pGameObject->GetComponent<PathFollow>();
			CalculateControlPointMatrices(pathFollow);
			CreateArcLengthTable(pathFollow);
			CreateDebugVAO(pathFollow);
		}
	}

	void PathFollowSystem::Update()
	{
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			PathFollow* pathFollow = mGameObjects[i]->GetComponent<PathFollow>();
			Body* pBody = mGameObjects[i]->GetComponent<Body>();
			pathFollow->mPathRunTime += FrameRateController::Instance().GetFrameTime();
			const double distance = pBody->mVelocity.z * pathFollow->mPathRunTime;
			const std::pair<float, int> searchedValue = SearchInArcTable(distance,pathFollow);
			glm::vec3 position = BSplineCurve::GetPointOnCurve(searchedValue.first, pathFollow->mControlPointsMatrices[searchedValue.second]);

			glm::mat4 mPathMatrix = glm::translate(glm::mat4(1.0f), position);
			
			glm::vec3 viewDirection = BSplineCurve::GetDerivativeOfPointOnCurve(searchedValue.first, pathFollow->mControlPointsMatrices[searchedValue.second]);
			viewDirection = glm::normalize(viewDirection);
			glm::vec3 wingDirection = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), viewDirection));
			glm::vec3 upDirection = glm::normalize(glm::cross(viewDirection, wingDirection));

			glm::mat4 rotation;
			rotation[0] = glm::vec4(wingDirection, 0.0f);
			rotation[1] = glm::vec4(upDirection, 0.0f);
			rotation[2] = glm::vec4(viewDirection, 0.0f);
			rotation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			pBody->mPosition.x = position.x;
			pBody->mPosition.z = position.z;
			pBody->mQuaternion = glm::toQuat(rotation);
		}
	}

	void PathFollowSystem::CalculateControlPointMatrices(PathFollow* pathFollow)
	{
		for (unsigned int i = 0; i <= pathFollow->mControlPoints.size() - 4; ++i)
		{
			glm::mat4 matrix;
			matrix[0] = pathFollow->mControlPoints[i];
			matrix[1] = pathFollow->mControlPoints[i + 1];
			matrix[2] = pathFollow->mControlPoints[i + 2];
			matrix[3] = pathFollow->mControlPoints[i + 3];

			pathFollow->mControlPointsMatrices.emplace_back(glm::transpose(matrix));
		}
	}

	void PathFollowSystem::CreateArcLengthTable(PathFollow* pathFollow)
	{
		pathFollow->mArcLengthTable.emplace_back(MAKE_ARCTABLE_ENTRY(0.0f, 0.0f, 0));
		for (unsigned int i = 0; i < pathFollow->mControlPointsMatrices.size(); ++i)
		{
			std::stack<ARCTABLE_ENTRY> adaptiveStack;
			adaptiveStack.push(MAKE_ARCTABLE_ENTRY(0.0f, 1.0f, i));

			while (adaptiveStack.size() > 0)
			{
				const ARCTABLE_ENTRY topValue = adaptiveStack.top();
				adaptiveStack.pop();

				int index = topValue.second.second;
				double sa = topValue.first;
				double sb = topValue.second.first;
				double sm = (sa + sb) / 2;

				glm::vec4 Psm = BSplineCurve::GetPointOnCurve(sm, pathFollow->mControlPointsMatrices[index]);
				glm::vec4 Psa = BSplineCurve::GetPointOnCurve(sa, pathFollow->mControlPointsMatrices[index]);
				glm::vec4 Psb = BSplineCurve::GetPointOnCurve(sb, pathFollow->mControlPointsMatrices[index]);

				float A = glm::length(Psm - Psa);
				float B = glm::length(Psb - Psm);
				float C = glm::length(Psb - Psa);

				if (A + B - C > pathFollow->mPathTolerance)
				{
					adaptiveStack.push(MAKE_ARCTABLE_ENTRY(sm, sb, index));
					adaptiveStack.push(MAKE_ARCTABLE_ENTRY(sa, sm, index));
				}
				else
				{
					unsigned int prevIndex = pathFollow->mArcLengthTable.size();
					float distance = pathFollow->mArcLengthTable[prevIndex - 1].first;
					pathFollow->mArcLengthTable.emplace_back(MAKE_ARCTABLE_ENTRY(distance + A, sm, index));
					pathFollow->mArcLengthTable.emplace_back(MAKE_ARCTABLE_ENTRY(distance + A + B, sb, index));
				}
			}
		}
	}

	void PathFollowSystem::CreateDebugVAO(PathFollow* pathFollow)
	{
		std::vector<glm::vec4> curvePoints;
		for (unsigned int j = 0; j < pathFollow->mControlPointsMatrices.size(); ++j)
		{
			for (float i = 0.0f; i <= 1.0f; i += 0.01f)
			{
				glm::vec4 pointOnCurve = BSplineCurve::GetPointOnCurve(i, pathFollow->mControlPointsMatrices[j]);
				pointOnCurve.y = 0.5f;
				curvePoints.emplace_back(pointOnCurve);
				curvePoints.emplace_back(pointOnCurve);
			}
		}
		
		VertexArray* va = new VertexArray();
		VertexBuffer vb;

		va->AddBuffer(vb);
		vb.AddData(&curvePoints[0], curvePoints.size() , sizeof(glm::vec4));

		va->Push(4, GL_FLOAT, sizeof(float));
		va->AddLayout();

		vb.Unbind();
		va->Unbind();
		
		pathFollow->mCurvePointsCount = curvePoints.size();
		pathFollow->mpCurveVAO = va;
	}

	std::pair<float, int> PathFollowSystem::SearchInArcTable(const float distance,PathFollow* pathFollow)
	{
		auto& table = pathFollow->mArcLengthTable;
		for (unsigned int i = 0; i < table.size(); ++i)
		{
			float tableDistance = table[i].first;
			if (tableDistance > distance)
			{
				float factor = (distance - table[i - 1].first) / (tableDistance - table[i - 1].first);
				float s; int index;
				if (table[i - 1].second.second == table[i].second.second)
				{
					float T2 = table[i].second.first;
					float T1 = table[i - 1].second.first;
					s = glm::lerp(T1, T2, factor);
					index = i;
				}
				else
				{
					s = table[i - 1].second.first;
					index = i - 1;
				}
				return std::make_pair(s, table[index].second.second);
			}
		}
		pathFollow->mPathRunTime = 0.0f;
		return std::make_pair(0.0f, 0);
	}
}
