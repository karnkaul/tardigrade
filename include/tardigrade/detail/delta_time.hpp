#pragma once
#include <chrono>
#include <utility>

namespace tg {
namespace stdch = std::chrono;
using namespace std::chrono_literals;

using Clock = stdch::steady_clock;
using Time = stdch::duration<float>;

inline Clock::time_point now() { return Clock::now(); }
inline Time diff(Clock::time_point from, Clock::time_point to = now()) { return to - from; }
inline Time diff_exchg(Clock::time_point& from, Clock::time_point to = now()) { return to - std::exchange(from, to); }

struct DeltaTime {
	Time real{};
	Time scaled{};

	constexpr DeltaTime(Time dt = {}, float scale = 1.0f) : real(dt), scaled(real * scale) {}
};
} // namespace tg
