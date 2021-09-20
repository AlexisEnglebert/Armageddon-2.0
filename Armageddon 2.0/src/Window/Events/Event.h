#pragma once

enum class EventType
{
	invalid,
	KeyPressed,
	KeyReleased,
	MousePressed,
	MouseReleased,
	MouseMove,
	MouseScroll,
	WindowResize,
	WindowMoved,
	WindowClose,
};
class Event
{
public:	
	EventType GetEventType() { return type;};
	bool Handle = false;
	EventType type;
};