#pragma once
#include "../../Macros.h"
struct MousePoint
{
	int x, y;
};

class DECL MouseEvent
{
public:
	enum class MEventType
	{
		Lpress,
		Lrelease,
		Rpress,
		Rrelease,
		Mpress,
		Mrelease,
		Wheelup,
		Wheeldown,
		Move,
		RAW_MOVE,
		Invalid
	};


	MouseEvent(); 
	MouseEvent(const MEventType, const int x, const int y);
	bool IsValid() const;
	MEventType getType();
	MousePoint getPoint();
	int GetX();
	int GetY();



private:
	MEventType type;
	int x, y;
};
