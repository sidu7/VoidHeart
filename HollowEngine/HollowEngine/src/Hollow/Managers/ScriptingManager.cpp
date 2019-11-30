#include <hollowpch.h>
#include "ScriptingManager.h"

#include "Hollow/Components/Body.h"
#include "Hollow/Components/Camera.h"
#include "Hollow/Components/Transform.h"

#include "Hollow/Managers/ResourceManager.h"

namespace Hollow
{
	void ScriptingManager::Init()
	{
		lua.open_libraries(sol::lib::base, sol::lib::math);

		auto mult_overloads = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 { return v1 * v2; },
			[](const glm::vec3& v1, float f) -> glm::vec3 { return v1 * f; },
			[](float f, const glm::vec3& v1) -> glm::vec3 { return f * v1; }
		);
		
		lua.new_usertype<glm::vec3>("vec3",
			sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
			"x", &glm::vec3::x,
			"y", &glm::vec3::y,
			"z", &glm::vec3::z,
			"__add", [](const glm::vec3& l, const glm::vec3& r) { return glm::vec3(l.x + r.x, l.y + r.y, l.z + r.z); },
			"__sub", [](const glm::vec3& l, const glm::vec3& r) { return glm::vec3(l.x - r.x, l.y - r.y, l.z - r.z); },
			sol::meta_function::multiplication, mult_overloads
			);

		lua.new_usertype<glm::vec2>("vec2",
			sol::constructors<glm::vec2(), glm::vec2(float), glm::vec2(float, float)>(),
			"x", &glm::vec2::x,
			"y", &glm::vec2::y,
			"__add", [](const glm::vec2& l, const glm::vec2& r) { return glm::vec2(l.x + r.x, l.y + r.y); },
			"__sub", [](const glm::vec2& l, const glm::vec2& r) { return glm::vec2(l.x - r.x, l.y - r.y); }
			);

		lua.new_usertype<Body>("RigidBody",
			sol::constructors<Body()>(),
			"velocity", &Body::mVelocity,
			"angularVelocity", &Body::mAngularVelocity,
			"position", &Body::mPosition
		);

		lua.new_usertype<Transform>("Transform",
			sol::constructors<Transform()>(),
			"position", &Transform::mPosition
			);

		lua.new_usertype<Camera>("Camera",
			sol::constructors<Camera()>(),
			"frontDirection", &Camera::mFront,
			"rightDirection", &Camera::mRight,
			"yaw", &Camera::mYaw,
			"pitch", &Camera::mPitch
			);

		lua.new_usertype<GameObject>("GameObject",
			sol::constructors<GameObject()>(),
			"GetBody", &GameObject::GetComponent<Body>,
			"GetTransform", &GameObject::GetComponent<Transform>
			);

		lua.set_function("CreateGameObject", &ResourceManager::LoadGameObjectFromFile, std::ref(ResourceManager::Instance()));
	}

}
