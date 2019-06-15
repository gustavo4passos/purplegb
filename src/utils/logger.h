#pragma once

#include <string>

namespace pgb
{
	#define ERROR_STRING(x) std::string() + "Error: " + __FILE__ + " : " + std::to_string(__LINE__) + " " + __func__ + " [" + x + "]"
	#define WARNING_STRING(x) std::string() + "Warning: " + __FILE__ + " : " + std::to_string(__LINE__) + " " + __func__ + " [" + x + "]"
}
