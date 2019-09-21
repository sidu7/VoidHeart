#include <Hollow.h>

#include "Hollow/Core/EntryPoint.h"

class Prototype0 : public Hollow::Application
{
public:
	Prototype0()
	{
	}
	
	~Prototype0()
	{
	}
};


Hollow::Application* Hollow::CreateApplication()
{
	return new Prototype0();
}