#pragma once

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