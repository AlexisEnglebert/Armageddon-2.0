#pragma once
#include "Event.h"
#include "../../Macros.h"

class DECL KeyBoardEvents : public Event
{
public:
	KeyBoardEvents();
	KeyBoardEvents(EventType type, const unsigned char key);
	bool Ispressed();
	bool IsReleased();
	bool IsInvalid();
private:
	EventType type;
	unsigned char key;
};