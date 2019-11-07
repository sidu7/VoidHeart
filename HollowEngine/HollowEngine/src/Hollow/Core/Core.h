#pragma once

#ifdef HW_PLATFORM_WINDOWS
	#ifdef HW_BUILD_DLL
		#define HOLLOW_API __declspec(dllexport)
	#else
		#define HOLLOW_API __declspec(dllimport)
	#endif
#else
	#error Hollow supports windows
#endif

#define BIT(x) (1<<x)

#define REGISTERCOMPONENT(x) public:										\
					x() : Component(#x, this) {}								\
					x(std::string name) : Component(name) {}				\
					std::type_index GetIndex()								\
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
					x() : GameEvent(#x) {}																								


#define REGISTERSYSTEM(x,y)	private:								\
							static x instance;						\
							x() : System(this,y,std::type_index(typeid(x))) { }