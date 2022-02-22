#pragma once
#include "../Log.h"
#include "../Macros.h"
#include "Events/KeyBoardEvents.h"
#include <queue>

class DECL KeyBoard
{
public:
	KeyBoard();
	bool KeyIsPressed(const unsigned char keycode);
	bool KeyBufferIsEmpty();
	bool CharBufferIsEmpty();
	KeyBoardEvents ReadKey();
	unsigned char ReadChar();
	void OnKeyPressed(const unsigned char key);
	void OnKeyReleased(const unsigned char key);
	void OnChar(const unsigned char key);
	bool keystate[256];

private:
	bool autoRepeatKey = false;
	bool autoRepeatChar = false;
	std::queue<KeyBoardEvents> keyBuffer; // on crée une queue qui vas stock  l'event
	std::queue<unsigned char> charBuffer; // on crée un queue qui vas stock le caractère
};