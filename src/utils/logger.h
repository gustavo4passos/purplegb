#pragma once

#include <iostream>
#include <string>

namespace pgb
{

class Logger
{
public:
	static auto GenErrorMessage(const std::string& message) -> std::string;
	static auto LogError(const std::string& message) -> void;
		
	static auto GenWarningMessage(const std::string& message) -> std::string;
	static auto LogWarning(const std::string& message) -> void;
};

}
