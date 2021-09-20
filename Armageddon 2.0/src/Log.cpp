#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace Armageddon 
{
	std::shared_ptr<spdlog::logger> Log::c_Logger;

	void Log::Init()
	{
		spdlog::set_pattern("%^ %n [%T] : %v	%$");
		c_Logger = spdlog::stdout_color_mt("Armageddon");
		c_Logger->set_level(spdlog::level::trace);
	}
}
