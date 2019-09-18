#pragma once

#include "Profiler.h"
#include "Singleton.h"

class Test : public Singleton<Test>
{
	float sum;
public:
	int work()
	{
		sum = 1.0f;

		{
			Timer time;
			for (int i = 1; i < 1000000; ++i)
			{
				sum += pow(i, 3) * log(sum);
			}
		}
		return sum;
	}

	
};