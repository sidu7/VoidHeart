#pragma once

// Use this to create Singletons easily (Not too many)
// The inherited class needs to set the Singleton instantiation as a friend
// Ex. class Test : public Singleton<Test>
// { friend class Singleton<Test>;
// ... }

template<class T>
class Singleton
{
protected:
	Singleton() {}
public:

	Singleton(Singleton const&) = delete;
	void operator=(Singleton const&) = delete;

	static T& Instance()
	{
		static T instance;
		return instance;
	}
};