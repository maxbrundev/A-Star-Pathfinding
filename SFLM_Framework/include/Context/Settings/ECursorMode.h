#pragma once

#include <cstdint>

namespace Context::Settings
{
	enum class ECursorMode : uint8_t
	{
		FREE    = 0,
		GRABBED = 1,
	};
}