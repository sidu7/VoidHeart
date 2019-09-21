#pragma once
#include "Hollow.h"
#include "Hollow/Components/Component.h"

class PrototypeComponent : public Hollow::Component
{
	REGISTER(PrototypeComponent);
public:
	PrototypeComponent();
	virtual ~PrototypeComponent();
	void Clear();

};