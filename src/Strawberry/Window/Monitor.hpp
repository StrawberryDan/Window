#pragma once

#include <vector>

#include "Strawberry/Core/Math/Vector.hpp"


namespace Strawberry::Window
{
	struct MonitorInfo
	{
		Core::Math::Vec2u mResolution;
		Core::Math::Vec2u mPhysicalSizeMM;


		Core::Math::Vec2f GetDPI() const noexcept;
	};


	std::vector<MonitorInfo> GetMonitorInfo();
}
