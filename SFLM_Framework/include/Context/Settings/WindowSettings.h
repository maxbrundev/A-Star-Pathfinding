#pragma once

#include <cstdint>

namespace Context::Settings
{
	struct WindowSettings
	{
		const char* Title;
		
		uint16_t Width;
		uint16_t Height;
		
		bool FullScreen = false;
		bool Resizable	= true;
		bool TitleBar   = true;
		bool Closable   = true;
	};
}
