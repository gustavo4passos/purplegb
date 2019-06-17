#include "logger.h"

auto pgb::Logger::GenErrorMessage(const std::string& message) -> std::string
{
	return std::string() + "Error: " + __FILE__ + " : " + 
		std::to_string(__LINE__) + " " + __func__ + " [" + message + 
		"]";
}

auto pgb::Logger::LogError(const std::string& message) -> void
{
	std::cout << GenErrorMessage(message) << '\n';
}

auto pgb::Logger::GenWarningMessage(const std::string& message) -> std::string
{
	return std::string() + "Warning: " + __FILE__ + " : " + 
		std::to_string(__LINE__) + " " + __func__ + " [" + message + 
			"]";
}

auto pgb::Logger::LogWarning(const std::string& message) -> void
{
	std::cout << GenWarningMessage(message) << '\n';
}

