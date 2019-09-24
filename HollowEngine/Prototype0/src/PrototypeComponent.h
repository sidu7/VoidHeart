#pragma once
#include "Hollow.h"
#include "Hollow/Components/Component.h"

class PrototypeComponent : public Hollow::Component
{
	REGISTERCOMPONENT(PrototypeComponent,"PrototypeComponent");
public:
	void Init();
	virtual ~PrototypeComponent();
	void Clear();
	void DebugDisplay();

};