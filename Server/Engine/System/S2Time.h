#pragma once

namespace s2 {

namespace S2Time
{
	inline time_t Now()
	{
		return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	}
};

};