#pragma once

// Use this macro inside class to make a class singleton
// class Example {
//		SINGLETON(Example)
//		... rest of the class stuff

#define SINGLETON(x) private: x() {}		\
public: x(const x&) = delete;				\
		x& operator= (const x&) = delete;	\
		x(x &&) = delete;					\
		x & operator=(x &&) = delete;		\
		static x& Instance()				\
		{									\
			static x instance;				\
			return instance;				\
		}
