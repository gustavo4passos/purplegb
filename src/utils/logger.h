#pragma once

#include <iostream>
#include <string>

namespace pgb
{
	#define ERROR_STRING(x) std::string() + "Error: " + __FILE__ + " : " + std::to_string(__LINE__) + " " + __func__ + " [" + x + "]"
	#define WARNING_STRING(x) std::string() + "Warning: " + __FILE__ + " : " + std::to_string(__LINE__) + " " + __func__ + " [" + x + "]"
	#define LOG_STRING(x) std::string() + "Log: " + __FILE__ + " : " + std::to_string(__LINE__) + " " + __func__ + " [" + x + "]"

	#define LOG_WARNING(x) std::cout << ERROR_STRING(x) << '\n'
	#define LOG(x) std::cout << LOG_STRING(x) << '\n'
}
