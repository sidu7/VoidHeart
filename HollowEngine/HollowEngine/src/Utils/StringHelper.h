#pragma once
#include "Hollow/Core/Core.h"
#include <string>
#include <cstring>
#include <vector>

namespace Hollow
{
	class HOLLOW_API StringHelper
	{
	public:
		inline static std::vector<std::string> Split(std::string content, std::string delimiter)
		{			
			std::vector<std::string> tokens;
			auto start = 0U;
			auto end = content.find(delimiter);
			while (end != std::string::npos)
			{
				tokens.emplace_back(content.substr(start, end - start));
				start = end + delimiter.length();
				end = content.find(delimiter, start);
			}

			return tokens;
		}

	};
}