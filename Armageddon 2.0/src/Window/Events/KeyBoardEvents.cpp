#include "KeyBoardEvents.h"

KeyBoardEvents::KeyBoardEvents() : type(EventType::invalid), key(0u)
{
}

KeyBoardEvents::KeyBoardEvents(EventType type, const unsigned char key) : type(type), key(key)
{
}

bool KeyBoardEvents::Ispressed()
{
    return this->type == EventType::KeyPressed;
}

bool KeyBoardEvents::IsReleased()
{
    return this->type == EventType::KeyReleased;
}

bool KeyBoardEvents::IsInvalid()
{
    return this->type == EventType::invalid;
}
