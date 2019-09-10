#include <iostream>

namespace Test
{
	__declspec(dllimport) void Print();
}

int main()
{
	Test::Print();
	while (true)
	{
	}
	return 0;
}