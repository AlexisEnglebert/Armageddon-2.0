#pragma once

#include "KeyBoard.h"
#include "Events/MouseEvent.h"


namespace Armageddon
{
    class WindowInterface
    {
        public:
            WindowInterface() = default;
            ~WindowInterface() = default;
            // Shouldn't be override
		    inline KeyBoard& GetKeyBoard() { return m_KeyBoard;	};
            float GetAspectRatio()		   { return(w_width / w_height); };

        protected:
            float w_height, w_width;
            std::wstring w_title , w_class = L"";

            float m_old_mouse_posX = 0.0f;
		    float m_old_mouse_posY = 0.0f;

            /*KeyBoard Handler*/
		    KeyBoard m_KeyBoard;

            bool ProcessMessage();
            std::function<void(MouseEvent::MEventType e, float x, float y)> MouseCallBack;
            std::function<void(const unsigned char Key)> KeyBoardCallBack;

    };
}