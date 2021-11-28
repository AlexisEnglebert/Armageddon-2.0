#include "Timer.h"
std::map<std::string, float> ProfilerData::m_profilerData;

ProfilerData::ProfilerData()
{
}

void ProfilerData::Clear()
{
	ProfilerData::m_profilerData.clear();

}
//		