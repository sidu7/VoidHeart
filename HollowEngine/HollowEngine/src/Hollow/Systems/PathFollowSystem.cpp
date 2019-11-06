#include <hollowpch.h>
#include "PathFollowSystem.h"

#include "Hollow/Components/PathFollow.h"
#include "Hollow/Components/Body.h"

#include "Hollow/Managers/FrameRateController.h"
#include "Hollow/Managers/RenderManager.h"

#include "Hollow/Graphics/VertexArray.h"
#include "Hollow/Graphics/VertexBuffer.h"

#include "Utils/BSplineCurve.h"

#include "Hollow/Managers/UIManager.h"

namespace Hollow
{
	PathFollowSystem PathFollowSystem::instance;
	
	void PathFollowSystem::Init()
	{
		StopMoving = false;
		UIManager::Instance().AddButtonFunction(UIButton::ButtonFunction::STOP_MOVING, BUTTON_FUNCTION(PathFollowSystem::StopPathMotion));
		UIManager::Instance().AddButtonFunction(UIButton::ButtonFunction::START_MOVING, BUTTON_FUNCTION(PathFollowSystem::StartPathMotion));
	}

	void PathFollowSystem::AddGameObject(GameObject* pGameObject)
	{
		if(CheckAllComponents<PathFollow,Body>(pGameObject))
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

			if(pathFollow->mMove && !StopMoving)
			{
				pathFollow->mPathRunTime += FrameRateController::Instance().GetFrameTime();
			}
			
			Body* pBody = mGameObjects[i]->GetComponent<Body>();

			// Debug
			if(pathFollow->mControlPointsChanged)
			{
				CalculateControlPointMatrices(pathFollow);
				CreateArcLengthTable(pathFollow);
				CreateDebugVAO(pathFollow);
				pathFollow->mControlPointsChanged = false;
			}
			if (pathFollow->mDebugPath)
			{
				DebugPathData pathData;
				pathData.mCurveVAO = pathFollow->mpCurveVAO;
				pathData.mCurvePointsCount = pathFollow->mCurvePointsCount;
				RenderManager::Instance().mDebugPathData.push_back(pathData);
			}
			
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

			if (pBody->bodyType == Body::KINEMATIC)
			{
				pBody->mPosition = position;
			}
			else
			{
				pBody->mPosition.x = position.x;
				pBody->mPosition.z = position.z;
			}
			pBody->mQuaternion = glm::toQuat(rotation);
		}
	}

	void PathFollowSystem::CalculateControlPointMatrices(PathFollow* pathFollow)
	{
		pathFollow->mControlPointsMatrices.clear();
		
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
		pathFollow->mArcLengthTable.clear();
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
		if (pathFollow->mpCurveVAO)
		{
			delete pathFollow->mpCurveVAO;
		}
		
		std::vector<glm::vec4> curvePoints;
		for (unsigned int j = 0; j < pathFollow->mControlPointsMatrices.size(); ++j)
		{
			for (float i = 0.0f; i <= 1.0f; i += 0.01f)
			{
				glm::vec4 pointOnCurve = BSplineCurve::GetPointOnCurve(i, pathFollow->mControlPointsMatrices[j]);
				//pointOnCurve.y = 0.5f;
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

	void PathFollowSystem::StopPathMotion()
	{
		StopMoving = true;
	}

	void PathFollowSystem::StartPathMotion()
	{
		StopMoving = false;
	}

	std::pair<float, int> PathFollowSystem::SearchInArcTable(const float distance,PathFollow* pathFollow)
	{
		auto& table = pathFollow->mArcLengthTable;
		int index = BinarySearch(0, table.size(), distance, table);

		if(index > 0)
		{
			float tableDistance = table[index].first;
			float factor = (distance - table[index - 1].first) / (tableDistance - table[index - 1].first);
			float s;
			if (table[index - 1].second.second == table[index].second.second)
			{
				float T2 = table[index].second.first;
				float T1 = table[index - 1].second.first;
				s = glm::lerp(T1, T2, factor);
			}
			else
			{
				s = table[index - 1].second.first;
				index = index - 1;
			}
			return std::make_pair(s, table[index].second.second);
		}
		else
		{
			pathFollow->mPathRunTime = 0.0f;
			return std::make_pair(0.0f, 0);
		}		
	}

	int PathFollowSystem::BinarySearch(unsigned int start, unsigned int end, float distance,
		std::vector<std::pair<float, std::pair<float, int>>>& list)
	{
		unsigned int mid = start + (end - start) / 2;
		if (distance == list[mid].first)
		{
			return mid;
		}
		else if (distance > list[mid].first)
		{
			if(mid == list.size() - 1)
			{
				return -1;
			}
			if (distance < list[mid + 1].first)
			{
				return mid + 1;
			}
			return BinarySearch(mid + 1, end, distance, list);
		}
		else if (distance < list[mid].first)
		{
			if(distance > list[mid - 1].first)
			{
				return mid;
			}
			return BinarySearch(start, mid - 1, distance, list);
		}
	}
}
