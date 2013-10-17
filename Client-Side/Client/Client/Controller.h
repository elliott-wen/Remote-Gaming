#include "stdafx.h"
#include <SDL.h>
#include <Windows.h>
#include <string>
#include "DataTunnel.h"
#include <map>
using namespace std;
#define KEYEVENT 1
#define MOUSEEVENT 2
#define PRESSDOWNDIRECTION 0
#define PRESSUPDIRECTION 1
#define INVALID_KEY -1
typedef WORD KeySym;	
struct ControlEvent
{
	int type;
	int keyCode1;
	int keyCode2;
	int relx;
	int rely;
	int clickedButton;
	int direction;
};
class Controller
{
public:
	Controller();
	~Controller();
	bool initController();
	bool sendKeyEvent(int key1,int key2,int direction);
	bool sendMouseEvent(int relx,int rely,int clickButton,int direction);
	void SetDataTunnel(DataTunnel *tunnel);
private:
	int sdlkeyCodeTransfer(unsigned short sdlKeyCode);
	DataTunnel *tunnel;
	bool sendOutEvent(char *event,int size);
	map<unsigned short, KeySym> keymap;
	void initKeyMap();

	
};

