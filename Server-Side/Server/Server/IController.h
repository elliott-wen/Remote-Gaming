#pragma once
#include "stdafx.h"
#include "IDataTunnel.h"
#include <windows.h>
#include <set>
using namespace std;
#define KEYEVENT 1
#define MOUSEEVENT 2
#define PRESSDOWNDIRECTION 0
#define PRESSUPDIRECTION 1
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
class IController
{
public:
	IController();
	~IController();
	bool initController();
	void startControllerLoop();
	void stopControllerLoop();
	void setDataTunnel(IDataTunnel *tunnel);
private:
	bool replayKeyboardEvent(int virtualKeyCode1,int virtualKeyCode2,int direction);
	bool replayMouseEvent(int relx,int rely,int button,int updown);
	bool runFlag;
	IDataTunnel *tunnel;
	set<int> pressedKeySet;
	void cleanPressedKeySet();
};