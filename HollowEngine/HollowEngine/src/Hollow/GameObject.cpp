#include "hollowpch.h"
#include "GameObject.h"
#include "Components/Component.h"

namespace Hollow {

	GameObject::GameObject(): mID(0), mActive(true)
	{
	}

	GameObject::~GameObject()
	{
	}

}