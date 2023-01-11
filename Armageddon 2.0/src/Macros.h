#pragma once

// This is to be sure that functions are in the symbol table
// We only need this for windows

#define WINDOWS _WIN32 || defined(WIN32) || defined(__WIN32__) || defined(__NT__)

#ifdef ARMAGEDDON_BUILD_DLL
    #if __linux__
        #define DECL
    #else
        #define DECL __declspec(dllexport)
    #endif
#else
    #if __linux__ 
        #define DECL
    #else
        #define DECL __declspec(dllimport)
    #endif
#endif


#define AG_GET_RENDERER() Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer()



#define AG_KEY_backspace	8	
#define AG_KEY_tab			9
#define AG_KEY_enter		13
#define AG_KEY_shift		16
#define AG_KEY_ctrl			17
#define AG_KEY_alt			18
#define AG_KEY_pause 		19
#define AG_KEY_caps_lock	20
#define AG_KEY_escape		27
#define AG_KEY_page_up		33
#define AG_KEY_Space		32
#define AG_KEY_page_down	34
#define AG_KEY_end			35
#define AG_KEY_home			36
#define AG_KEY_arrow_left	37 /* -> */
#define AG_KEY_arrow_up		38		/* ^ */
#define AG_KEY_arrow_right	39 /* <- */
#define AG_KEY_arrow_down	40 /* v */
#define AG_KEY_print_screen	44
#define AG_KEY_insert		45
#define AG_KEY_delete		46
#define AG_KEY_0			48
#define AG_KEY_1			49
#define AG_KEY_2			50
#define AG_KEY_3			51
#define AG_KEY_4			52
#define AG_KEY_5			53
#define AG_KEY_6			54
#define AG_KEY_7			55
#define AG_KEY_8			56
#define AG_KEY_9			57
#define AG_KEY_A			65
#define AG_KEY_B			66
#define AG_KEY_C			67
#define AG_KEY_D			68
#define AG_KEY_E			69
#define AG_KEY_F			70
#define AG_KEY_G			71
#define AG_KEY_H			72
#define AG_KEY_I			73
#define AG_KEY_J			74
#define AG_KEY_K			75
#define AG_KEY_L			76
#define AG_KEY_M			77
#define AG_KEY_N			78
#define AG_KEY_O			79
#define AG_KEY_P			80
#define AG_KEY_Q			81
#define AG_KEY_R			82
#define AG_KEY_S			83
#define AG_KEY_T			84
#define AG_KEY_U			85
#define AG_KEY_V			86
#define AG_KEY_W			87
#define AG_KEY_X			88
#define AG_KEY_Y			89
#define AG_KEY_Z			90
#define AG_KEY_WINDOWS1		91
#define AG_KEY_WINDOWS		92
#define AG_KEY_select 		93
#define AG_KEY_numpad_0		96
#define AG_KEY_numpad_1		97
#define AG_KEY_numpad_2		98
#define AG_KEY_numpad_3		99
#define AG_KEY_numpad_4		100
#define AG_KEY_numpad_5		101
#define AG_KEY_numpad_6		102
#define AG_KEY_numpad_7		103
#define AG_KEY_numpad_8		104
#define AG_KEY_numpad_9		105
#define AG_KEY_multiply		106
#define AG_KEY_add			107
#define AG_KEY_subtract		109
#define AG_KEY_decimal_point 110
#define AG_KEY_divide		111
#define AG_KEY_f1			112
#define AG_KEY_f2			113
#define AG_KEY_f3			114
#define AG_KEY_f4			115
#define AG_KEY_f5			116
#define AG_KEY_f6			117
#define AG_KEY_f7			118
#define AG_KEY_f8			119
#define AG_KEY_f9			120
#define AG_KEY_f10			121
#define AG_KEY_f11			122
#define AG_KEY_f12			123
#define AG_KEY_num_lock		144
#define AG_KEY_scroll_lock	145
#define AG_KEY_My 			182 /*Computer(multimedia keyboard)*/
#define AG_KEY_My2 			183 /*Calculator(multimedia keyboard)*/
#define AG_KEY_semicolon	186
#define AG_KEY_equalsign	187
#define AG_KEY_comma		188
#define AG_KEY_dash			189
#define AG_KEY_period		190
#define AG_KEY_forward_slash 191
#define AG_KEY_open_bracket	219
#define AG_KEY_back_slash	220
#define AG_KEY_close_braket	221
#define AG_KEY_single_quote	222