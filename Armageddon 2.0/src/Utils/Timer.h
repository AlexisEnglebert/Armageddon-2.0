#pragma once
#include "../Log.h"
#include <map>
class Timer
{
public:
	Timer()
	{
		ResetTimer();
	};
	void ResetTimer()
	{
		m_start_timer = std::chrono::high_resolution_clock::now();
	};
	float TimeElapsed()
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_start_timer).count() * 0.00001;
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_start_timer;
};

class DECL ProfilerData
{
public:
	ProfilerData();
	static void Clear();
	static std::map<std::string, float> m_profilerData;
private:
};
class Profiler
{
public:
	Profiler(std::string name) : m_name(name)
	{};
	~Profiler()
	{
		float time = m_timer.TimeElapsed();
		//Armageddon::Log::GetLogger()->info("[PROFILER] {0} - {1} ms", m_name, time);
		ProfilerData::m_profilerData.insert(std::make_pair(m_name, time));
	}
private:
	Timer m_timer;
	std::string m_name;

};

