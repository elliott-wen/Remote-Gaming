#include "Controller.h"
Controller::Controller()
{
	this->tunnel=0;
}
Controller::~Controller()
{
}

void Controller::SetDataTunnel(DataTunnel *tunnel)
{
	this->tunnel=tunnel;
}
bool Controller::sendKeyEvent(int key1,int key2,int direction)
{
	ControlEvent kevent;
	memset(&kevent,0,sizeof(kevent));
	kevent.type=KEYEVENT;
	int temp=sdlkeyCodeTransfer(key1);
	if(temp==-1)
	{
		printf("Ignorg this key event\n");
		return false;
	}
	kevent.keyCode1=temp;
	if(key2!=0)
	{
		temp=sdlkeyCodeTransfer(key2);
		if(temp!=-1)
			kevent.keyCode2=temp;
	}
	kevent.direction=direction;
	return sendOutEvent((char *)(&kevent),sizeof(kevent));
}
bool Controller::sendMouseEvent(int relx,int rely,int clickButton,int direction)
{
	ControlEvent mevent;
	memset(&mevent,0,sizeof(mevent));
	mevent.type=MOUSEEVENT;
	if(clickButton==0)
	{
		mevent.relx=relx;
		mevent.rely=rely;
	}
	else
	{
		mevent.clickedButton=clickButton;
		mevent.direction=direction;
	}
	return sendOutEvent((char *)(&mevent),sizeof(mevent));
}
bool Controller::sendOutEvent(char *data,int size)
{
	//int sendSize=sendto(fd,data,size,0,(const sockaddr *)&this->remoteAddr,sizeof(this->remoteAddr));
	//return sendSize==size;
	if(this->tunnel!=NULL&&tunnel->isServerConnected())
	{
		return this->tunnel->sendControllerData(data,size);
	}
	return false;
}
bool Controller::initController()
{
	initKeyMap();
	return true;
}
void Controller::initKeyMap()
{
	unsigned short i;
	//
	keymap[KMOD_LSHIFT] = VK_LSHIFT;
	keymap[KMOD_RSHIFT] = VK_RSHIFT;
	keymap[KMOD_LCTRL] = VK_LCONTROL;
	keymap[KMOD_RCTRL] = VK_RCONTROL;
	keymap[KMOD_LALT] = VK_LMENU;
	keymap[KMOD_RALT] = VK_RMENU;
	keymap[KMOD_RSHIFT] = VK_RSHIFT;
	keymap[SDLK_BACKSPACE]	= VK_BACK;		//		= 8,
	keymap[SDLK_TAB]	= VK_TAB;		//		= 9,
	keymap[SDLK_CLEAR]	= VK_CLEAR;		//		= 12,
	keymap[SDLK_RETURN]	= VK_RETURN;		//		= 13,
	keymap[SDLK_PAUSE]	= VK_PAUSE;		//		= 19,
	keymap[SDLK_ESCAPE]	= VK_ESCAPE;		//		= 27,
	// Latin 1: starting from space (0x20)
	keymap[SDLK_SPACE]	= VK_SPACE;		//		= 32,
	// (0x20) space, exclam, quotedbl, numbersign, dollar, percent, ampersand,
	// (0x27) quoteright, parentleft, parentright, asterisk, plus, comma,
	// (0x2d) minus, period, slash
	//SDLK_EXCLAIM		= 33,
	keymap[SDLK_QUOTEDBL]	= VK_OEM_7;		//		= 34,
	//SDLK_HASH		= 35,
	//SDLK_DOLLAR		= 36,
	//SDLK_AMPERSAND		= 38,
	keymap[SDLK_QUOTE]	= VK_OEM_7;		//		= 39,
	//SDLK_LEFTPAREN		= 40,
	//SDLK_RIGHTPAREN		= 41,
	//SDLK_ASTERISK		= 42,
	keymap[SDLK_PLUS]	= VK_OEM_PLUS;		//		= 43,
	keymap[SDLK_COMMA]	= VK_OEM_COMMA;		//		= 44,
	keymap[SDLK_MINUS]	= VK_OEM_MINUS;		//		= 45,
	keymap[SDLK_PERIOD]	= VK_OEM_PERIOD;	//		= 46,
	keymap[SDLK_SLASH]	= VK_OEM_2;		//		= 47,
	keymap[SDLK_COLON]	= VK_OEM_1;		//		= 58,
	keymap[SDLK_SEMICOLON]	= VK_OEM_1;		//		= 59,
	keymap[SDLK_LESS]	= VK_OEM_COMMA;		//		= 60,
	keymap[SDLK_EQUALS]	= VK_OEM_PLUS;		//		= 61,
	keymap[SDLK_GREATER]	= VK_OEM_PERIOD;	//		= 62,
	keymap[SDLK_QUESTION]	= VK_OEM_2;		//		= 63,
	//SDLK_AT			= 64,
	/* 
	   Skip uppercase letters
	 */
	keymap[SDLK_LEFTBRACKET]= VK_OEM_4;		//		= 91,
	keymap[SDLK_BACKSLASH]	= VK_OEM_5;		//		= 92,
	keymap[SDLK_RIGHTBRACKET]= VK_OEM_6;		//		= 93,
	//SDLK_CARET		= 94,
	keymap[SDLK_UNDERSCORE]	= VK_OEM_MINUS;		//		= 95,
	keymap[SDLK_BACKQUOTE]	= VK_OEM_3;		//		= 96,
	// (0x30-0x39) 0-9
	for(i = 0x30; i <= 0x39; i++) {
		keymap[i] = i;
	}
	// (0x3a) colon, semicolon, less, equal, greater, question, at
	// (0x41-0x5a) A-Z
	// SDL: no upper cases, only lower cases
	// (0x5b) bracketleft, backslash, bracketright, asciicircum/caret,
	// (0x5f) underscore, grave
	// (0x61-7a) a-z
	for(i = 0x61; i <= 0x7a; i++) {
		keymap[i] = i & 0xdf;	// convert to uppercases
	}
	keymap[SDLK_DELETE]	= VK_DELETE;		//		= 127,
	// SDLK_WORLD_0 (0xa0) - SDLK_WORLD_95 (0xff) are ignored
	/** @name Numeric keypad */
	keymap[SDLK_KP0]	= VK_NUMPAD0;	//		= 256,
	keymap[SDLK_KP1]	= VK_NUMPAD1;	//		= 257,
	keymap[SDLK_KP2]	= VK_NUMPAD2;	//		= 258,
	keymap[SDLK_KP3]	= VK_NUMPAD3;	//		= 259,
	keymap[SDLK_KP4]	= VK_NUMPAD4;	//		= 260,
	keymap[SDLK_KP5]	= VK_NUMPAD5;	//		= 261,
	keymap[SDLK_KP6]	= VK_NUMPAD6;	//		= 262,
	keymap[SDLK_KP7]	= VK_NUMPAD7;	//		= 263,
	keymap[SDLK_KP8]	= VK_NUMPAD8;	//		= 264,
	keymap[SDLK_KP9]	= VK_NUMPAD9;	//		= 265,
	keymap[SDLK_KP_PERIOD]	= VK_DECIMAL;	//		= 266,
	keymap[SDLK_KP_DIVIDE]	= VK_DIVIDE;	//		= 267,
	keymap[SDLK_KP_MULTIPLY]= VK_MULTIPLY;	//		= 268,
	keymap[SDLK_KP_MINUS]	= VK_SUBTRACT;	//		= 269,
	keymap[SDLK_KP_PLUS]	= VK_ADD;	//		= 270,
	//keymap[SDLK_KP_ENTER]	= XK_KP_Enter;	//		= 271,
	//keymap[SDLK_KP_EQUALS]	= XK_KP_Equal;	//		= 272,
	/** @name Arrows + Home/End pad */
	keymap[SDLK_UP]		= VK_UP;	//		= 273,
	keymap[SDLK_DOWN]	= VK_DOWN;	//		= 274,
	keymap[SDLK_RIGHT]	= VK_RIGHT;	//		= 275,
	keymap[SDLK_LEFT]	= VK_LEFT;	//		= 276,
	keymap[SDLK_INSERT]	= VK_INSERT;	//		= 277,
	keymap[SDLK_HOME]	= VK_HOME;	//		= 278,
	keymap[SDLK_END]	= VK_END;	//		= 279,
	keymap[SDLK_PAGEUP]	= VK_PRIOR;	//		= 280,
	keymap[SDLK_PAGEDOWN]	= VK_NEXT;	//		= 281,
	/** @name Function keys */
	keymap[SDLK_F1]		= VK_F1;	//		= 282,
	keymap[SDLK_F2]		= VK_F2;	//		= 283,
	keymap[SDLK_F3]		= VK_F3;	//		= 284,
	keymap[SDLK_F4]		= VK_F4;	//		= 285,
	keymap[SDLK_F5]		= VK_F5;	//		= 286,
	keymap[SDLK_F6]		= VK_F6;	//		= 287,
	keymap[SDLK_F7]		= VK_F7;	//		= 288,
	keymap[SDLK_F8]		= VK_F8;	//		= 289,
	keymap[SDLK_F9]		= VK_F9;	//		= 290,
	keymap[SDLK_F10]	= VK_F10;	//		= 291,
	keymap[SDLK_F11]	= VK_F11;	//		= 292,
	keymap[SDLK_F12]	= VK_F12;	//		= 293,
	keymap[SDLK_F13]	= VK_F13;	//		= 294,
	keymap[SDLK_F14]	= VK_F14;	//		= 295,
	keymap[SDLK_F15]	= VK_F15;	//		= 296,
	/** @name Key state modifier keys */
	keymap[SDLK_NUMLOCK]	= VK_NUMLOCK;	//		= 300,
	keymap[SDLK_CAPSLOCK]	= VK_CAPITAL;	//		= 301,
	keymap[SDLK_SCROLLOCK]	= VK_SCROLL;	//		= 302,
	keymap[SDLK_RSHIFT]	= VK_RSHIFT;	//		= 303,
	keymap[SDLK_LSHIFT]	= VK_LSHIFT;	//		= 304,
	keymap[SDLK_RCTRL]	= VK_RCONTROL;	//		= 305,
	keymap[SDLK_LCTRL]	= VK_LCONTROL;	//		= 306,
	keymap[SDLK_RALT]	= VK_RMENU;	//		= 307,
	keymap[SDLK_LALT]	= VK_LMENU;	//		= 308,
	keymap[SDLK_RMETA]	= VK_RWIN;	//		= 309,
	keymap[SDLK_LMETA]	= VK_LWIN;	//		= 310,
	//keymap[SDLK_LSUPER]	= XK_Super_L;	//		= 311,		/**< Left "Windows" key */
	//keymap[SDLK_RSUPER]	= XK_Super_R;	//		= 312,		/**< Right "Windows" key */
	keymap[SDLK_MODE]	= VK_MODECHANGE;//		= 313,		/**< "Alt Gr" key */
	//keymap[SDLK_COMPOSE]	= XK_Multi_key;	//		= 314,		/**< Multi-key compose key */
	/** @name Miscellaneous function keys */
	keymap[SDLK_HELP]	= VK_HELP;	//		= 315,
#if ! SDL_VERSION_ATLEAST(2,0,0)
	keymap[SDLK_PRINT]	= VK_PRINT;	//		= 316,
#endif
	//keymap[SDLK_SYSREQ]	= XK_Sys_Req;	//		= 317,
	keymap[SDLK_BREAK]	= VK_CANCEL;	//		= 318,
	keymap[SDLK_MENU]	= VK_MENU;	//		= 319,
	//keymap[SDLK_UNDO]	= XK_Undo;	//		= 322,		/**< Atari keyboard has Undo */
	//
	return;
}
int Controller::sdlkeyCodeTransfer(unsigned short sdlkey)
{
	map<unsigned short, KeySym>::iterator mi;
	if((mi = keymap.find(sdlkey)) != keymap.end()) {
		return mi->second;
	}
	return INVALID_KEY;
}
/*int Controller::sdlkeyCodeTransfer(int sdlKeyCode)
{
	
	if(sdlKeyCode>=SDLK_a&&sdlKeyCode<=SDLK_z)// captial
	{
		return 65+sdlKeyCode-SDLK_a;
	}
	if(sdlKeyCode>=SDLK_0&&sdlKeyCode<=SDLK_9)
	{
		return 48+sdlKeyCode-SDLK_0;
	}
	switch(sdlKeyCode)
	{
		case SDLK_ESCAPE:
			return VK_ESCAPE;
			
		case SDLK_CAPSLOCK:
			return VK_CAPITAL;
			
		case SDLK_RIGHT:
			return VK_RIGHT;
			
		case SDLK_LEFT:
			return VK_RIGHT;
			
		case SDLK_DOWN:
			return VK_DOWN;
			
		case SDLK_UP:
			return VK_UP;
		
		case SDLK_BACKSPACE:
			return VK_BACK;
			
		case SDLK_SPACE:
			return VK_SPACE;
			
		case SDLK_RETURN:
			return VK_RETURN;
			
		case SDLK_LCTRL:
		case SDLK_RCTRL:
		case KMOD_LCTRL:
		case KMOD_RCTRL:
			return VK_CONTROL;
			
		case SDLK_LSHIFT:
		case SDLK_RSHIFT:
		case KMOD_LSHIFT:
		case KMOD_RSHIFT:
			return VK_SHIFT;
			
		case SDLK_LALT:
		case SDLK_RALT:
		case KMOD_LALT:
		case KMOD_RALT:
			return VK_MENU;
			
		case SDLK_DELETE:
			return VK_DELETE;
			
		case SDLK_TAB:
			return VK_TAB;
			
		case SDLK_COLON:
		case SDLK_SEMICOLON:
			return VK_OEM_1;
			
		case SDLK_BACKQUOTE:
			return VK_OEM_3;
			
		case SDLK_COMMA:
			return VK_OEM_COMMA;
			
		case SDLK_PERIOD:
			return VK_OEM_PERIOD;
			
		case SDLK_SLASH:
			return VK_OEM_2;
			
		case SDLK_QUOTE:
			return VK_OEM_7;
			
		case SDLK_LEFTBRACKET:
			return VK_OEM_4;
			
		case SDLK_BACKSLASH:
			return VK_OEM_5;
			
		case SDLK_RIGHTBRACKET:
			return VK_OEM_6;
			
		case SDLK_MINUS:
			return VK_OEM_MINUS;
			
		case SDLK_EQUALS:
			return VK_OEM_PLUS;
			
			
		default:
			LOG4CPLUS_INFO(Logger::getInstance(TEXT("CLIENTLOG")),"Unsupport key\n");
			return -1;

	}
}*/
