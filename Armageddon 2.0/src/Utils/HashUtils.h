#pragma once
#include "../Macros.h"
#include "../Log.h"
#include <functional>
class DECL HashUtils
{
public:
	HashUtils() = default;
	~HashUtils() {};
	static uint64_t _64BitHash(std::string str)
	{
		return (uint64_t)std::hash<std::string>()(str);
	};
	static uint32_t _32BitHash(std::string str)
	{
		return (uint32_t)std::hash<std::string>()(str);
	};

	//TODO CRC32
private:
};