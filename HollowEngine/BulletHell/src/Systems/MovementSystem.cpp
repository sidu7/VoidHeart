#include <Hollow.h>
#include "MovementSystem.h"

#include "Components/Movement.h"

#include "Hollow/Components/Body.h"
#include "Hollow/Components/Transform.h"

#include "Hollow/Managers/ScriptingManager.h"
#include "Hollow/Managers/FrameRateController.h"
#include "Hollow/Managers/InputManager.h"
#include "Hollow/Managers/PhysicsManager.h"

namespace BulletHell
{
	MovementSystem MovementSystem::instance;

	void MovementSystem::Update()
	{
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			Movement* script = mGameObjects[i]->GetComponent<Movement>();
			Hollow::Body* pBody = mGameObjects[i]->GetComponent<Hollow::Body>();
			short rightHorizontalAxis = Hollow::InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_RIGHTX);
			short rightVerticallAxis = Hollow::InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_RIGHTY);
			auto& lua = Hollow::ScriptingManager::Instance().lua;
			lua["horizontalAxis"] = rightHorizontalAxis;
			lua["verticalAxis"] = rightVerticallAxis;
			int hThreshold = 8000;
			int vThreshold = 8000;
			//if (abs(rightHorizontalAxis) > hThreshold || abs(rightVerticallAxis) > hThreshold)
			//{
			//	float xVelocity = rightHorizontalAxis;
			//	float zVelocity = rightVerticallAxis;
			//	float angle;
			//	/*if(abs(xVelocity)>abs(zVelocity))
			//	{
			//		angle = xVelocity/ abs(xVelocity);
			//		angle *= 5.0f;
			//	}
			//	else
			//	{
			//		angle = zVelocity / abs(zVelocity);
			//		angle *= 5.0f;
			//	}*/
			//	
			//	float totalVelocity = sqrt(xVelocity * xVelocity + zVelocity * zVelocity);
			//	xVelocity = xVelocity / totalVelocity;
			//	zVelocity = zVelocity / totalVelocity;
			//	glm::vec3 direc(xVelocity, 0.0, zVelocity);
			//	float test = atan(zVelocity/ xVelocity);
			//	if (test < 0.0f)
			//	{
			//		if (zVelocity < 0.0f)
			//		{
			//			test = M_PI / 2.0f - test;
			//		}
			//		else if (xVelocity < 0.0f)
			//		{
			//			test = -(M_PI / 2.0f - abs(test));
			//		}
			//	}
			//	else
			//	{
			//		if(xVelocity > 0.0f && zVelocity > 0.0f)
			//		{
			//			test = M_PI / 2.0f - test;
			//		}
			//		else
			//		{
			//			test = -(M_PI / 2.0f + test);
			//		}
			//	}
			//	float yAngle;
			//	double sinp = 2 * (pBody->mQuaternion.w * pBody->mQuaternion.y - pBody->mQuaternion.z * pBody->mQuaternion.x);
			//	if (abs(sinp) >= 1)
			//		yAngle = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
			//	else
			//		yAngle = std::asin(sinp);

			//	HW_TRACE("Angle {0}", yAngle);
			//	Hollow::Transform* pTran = mGameObjects[i]->GetComponent<Hollow::Transform>();
			//	if(yAngle < test)
			//		pBody->mAngularVelocity = glm::vec3(0.0,20.0,0.0);
			//	else
			//		pBody->mAngularVelocity = glm::vec3(0.0,-20.0,0.0);
			//	
			//	
			//}


			lua["body"] = pBody;

			lua["isMoveForward"] = Hollow::InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_LEFTY) < -16000;

			lua["isMoveBackward"] = Hollow::InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_LEFTY) > 16000;

			lua["isStrafeLeft"] = Hollow::InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_LEFTX) < -16000;

			lua["isStrafeRight"] = Hollow::InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_LEFTX) > 16000;

			//lua["dash"] = (Hollow::InputManager::Instance().IsControllerButtonTriggered(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER));
			//lua["dash"] = (Hollow::InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_TRIGGERLEFT) > 5000);
			lua["dash"] = (Hollow::InputManager::Instance().IsControllerTriggerTriggered(SDL_CONTROLLER_AXIS_TRIGGERLEFT));

			lua.script_file(script->mScriptPath);
			glm::vec3 impulse = lua.get<glm::vec3>("impulse");
			Hollow::PhysicsManager::Instance().ApplyLinearImpulse(mGameObjects[i], impulse);
		}
	}

	void MovementSystem::AddGameObject(Hollow::GameObject* pGameObject)
	{
		CheckAllComponents<Movement, Hollow::Body>(pGameObject);
	}
}