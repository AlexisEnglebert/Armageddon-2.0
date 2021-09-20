#include "MouseEvent.h"

MouseEvent::MouseEvent() : type(MEventType::Invalid), x(0), y(0)
{
}

MouseEvent::MouseEvent(const MEventType type, const int x, const int y) : type(type), x(x), y(y)
{
}

bool MouseEvent::IsValid() const
{
	return this->type != MEventType::Invalid;
}

MouseEvent::MEventType MouseEvent::getType()
{
	return this->type;
}

MousePoint MouseEvent::getPoint()
{
	return { this->x,this->y };

}

int MouseEvent::GetX()
{
	return this->x;
}

int MouseEvent::GetY()
{
	return this->y;
}