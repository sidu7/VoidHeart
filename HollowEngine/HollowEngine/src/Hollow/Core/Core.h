#pragma once

#ifdef HW_PLATFORM_WINDOWS
	#ifdef HW_BUILD_DLL
		#define HOLLOW_API __declspec(dllexport)
		#define HOLLOW_API_GAME __declspec(dllexport)
	#else
		#define HOLLOW_API __declspec(dllimport)
		#define HOLLOW_API_GAME
	#endif
#else
	#error Hollow supports windows
#endif

#define BIT(x) (1<<x)

#define REGISTERCOMPONENT(x) public:										\
					HOLLOW_API_GAME x() : Component(#x, this) {}								\
					HOLLOW_API_GAME x(std::string name) : Component(name) {}				\
					HOLLOW_API_GAME std::type_index GetIndex()								\
					{														\
						return std::type_index(typeid(x));					\
					}														\
					private:												\
					static x instance;										\
					Component* CreateComponent()							\
					{														\
						return new x(mComponentName);						\
					}

#define REGISTEREVENT(x) public:										\
					HOLLOW_API_GAME x() : GameEvent(#x) {}																								


#define REGISTERSYSTEM(x,y)	private:								\
							static x instance;						\
							x() : System(this,y,std::type_index(typeid(x))) { }