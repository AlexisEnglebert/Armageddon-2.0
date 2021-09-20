#include "KeyBoard.h"

KeyBoard::KeyBoard()
{
    for (int i = 0; i < 256; i++)
    {
        this->keystate[i] = false; //Initialize all key states to off (false)
    }
}

bool KeyBoard::KeyIsPressed(const unsigned char keycode)
{
    return this->keystate[keycode];
}

bool KeyBoard::KeyBufferIsEmpty()
{
    return this->keyBuffer.empty();
}

bool KeyBoard::CharBufferIsEmpty()
{
    return this->charBuffer.empty();
}

KeyBoardEvents KeyBoard::ReadKey()
{
	if (this->keyBuffer.empty()) //If no keys to be read?
	{
		return KeyBoardEvents(); //return empty keyboard event
	}
	else
	{
		KeyBoardEvents e = this->keyBuffer.front(); //Get first Keyboard Event from queue
		this->keyBuffer.pop(); //Remove first item from queue
		return e; //Returns keyboard event
	}
}

unsigned char KeyBoard::ReadChar()
{
	if (this->charBuffer.empty()) //If no keys to be read?
	{
		return 0u; //return 0 (NULL char)
	}
	else
	{
		unsigned char e = this->charBuffer.front(); //Get first char from queue
		this->charBuffer.pop(); //Remove first char from queue
		return e; //Returns char
	}
}

void KeyBoard::OnKeyPressed(const unsigned char key)
{
	this->keystate[key] = true;
	this->keyBuffer.push(KeyBoardEvents(EventType::KeyPressed, key));
}

void KeyBoard::OnKeyReleased(const unsigned char key)
{
	this->keystate[key] = false;
	this->keyBuffer.push(KeyBoardEvents(EventType::KeyReleased, key));
}

void KeyBoard::OnChar(const unsigned char key)
{
	this->charBuffer.push(key);
}
