#include <Hollow.h>

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

int main()
{
	Prototype0* app = new Prototype0();
	app->Run();
	delete app;
}