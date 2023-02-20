#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <stdio.h>
#include <memory>
#include <iostream>
#include "Macros.h"















namespace Armageddon
{
	class DECL Log
	{
	public:

		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetLogger()
		{
			return c_Logger;
		}
	private:
		static std::shared_ptr<spdlog::logger> c_Logger;

	};
}