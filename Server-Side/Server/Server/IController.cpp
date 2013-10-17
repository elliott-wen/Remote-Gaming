#include "IController.h"
IController::IController()
{
	this->tunnel=0;
	this->runFlag=false;
}
IController::~IController()
{
}
bool IController::initController()
{
	runFlag=true;
	return true;
}
void IController::setDataTunnel(IDataTunnel *tunnel)
{
	this->tunnel=tunnel;
}
void IController::startControllerLoop()
{
	char *dataBuf;
	int getSize;
	
	while(runFlag)
	{
		
		if(tunnel!=NULL&&tunnel->isClientConnected())
		{
			
			if(!tunnel->getControllerData(&dataBuf,&getSize))
			{
				Sleep(ANTISPIN);
				continue;
			}
			
			if(getSize!=sizeof(ControlEvent))
			{
				printf( "Bad procotol\n" );
				break;
			}
			ControlEvent *cevent=(ControlEvent*)dataBuf;
			if(cevent->type==KEYEVENT)
			{
				printf("Got Keyevent %d %d %d\n",cevent->keyCode1, cevent->keyCode2,cevent->direction);
				this->replayKeyboardEvent(cevent->keyCode1,cevent->keyCode2,cevent->direction);
			}
			else if(cevent->type==MOUSEEVENT)
			{
				printf("Got Mouseevent %d %d %d %d\n",cevent->relx,cevent->rely,cevent->clickedButton,cevent->direction);
				
				this->replayMouseEvent(cevent->relx,cevent->rely,cevent->clickedButton,cevent->direction);
			}
			free(dataBuf);
		}
		else
		{
			Sleep(ANTISPIN);
		}
	}
	cleanPressedKeySet();
}
void IController::stopControllerLoop()
{
	this->runFlag=false;
}



bool IController::replayKeyboardEvent(int virtualKeyCode1,int virtualKeyCode2,int direction)
{
	
	if(virtualKeyCode2!=0)
	{
		 keybd_event(virtualKeyCode2,MapVirtualKey(virtualKeyCode2, 0),0,0);
		 keybd_event(virtualKeyCode1,MapVirtualKey(virtualKeyCode1, 0),0,0);
		 Sleep(10);
		 keybd_event(virtualKeyCode1,MapVirtualKey(virtualKeyCode1, 0),KEYEVENTF_KEYUP,0);
		 keybd_event(virtualKeyCode2,MapVirtualKey(virtualKeyCode2, 0),KEYEVENTF_KEYUP,0);
	}
	else
	{
		if(direction==PRESSDOWNDIRECTION)
		{
			keybd_event(virtualKeyCode1,MapVirtualKey(virtualKeyCode1, 0),0,0);
			this->pressedKeySet.insert(virtualKeyCode1);
		}
		else
		{
			keybd_event(virtualKeyCode1,MapVirtualKey(virtualKeyCode1, 0),KEYEVENTF_KEYUP,0);
			this->pressedKeySet.erase(virtualKeyCode1);
		}
	}
	return true;

}
void IController::cleanPressedKeySet()
{
	set<int>::const_iterator b=this->pressedKeySet.begin();
	for(; b!=this->pressedKeySet.end(); ++b)
	{
       keybd_event(*b,MapVirtualKey(*b, 0),KEYEVENTF_KEYUP,0);
	}
	this->pressedKeySet.clear();
}
bool IController::replayMouseEvent(int relx,int rely,int button,int direction)
{
	if(button==0)
	{
		mouse_event(MOUSEEVENTF_MOVE,relx,rely,0,0);
	}
	else
	{
		switch(button)
		{
			case 1:
				if(direction==PRESSDOWNDIRECTION )
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
				}
				else
				{
					mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
				}
			break;
			case 3:
				if(direction==PRESSDOWNDIRECTION )
				{
					mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
				}
				else
				{
					mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
				}
			break;
			case 2:
				if(direction==PRESSDOWNDIRECTION )
				{
					mouse_event(MOUSEEVENTF_MIDDLEDOWN,0,0,0,0);
				}
				else
				{
					mouse_event(MOUSEEVENTF_MIDDLEUP,0,0,0,0);
				}
			break;
			case 4:
				mouse_event(MOUSEEVENTF_WHEEL,0,0,WHEEL_DELTA,0);
				break;
			case 5:
				mouse_event(MOUSEEVENTF_WHEEL,0,0,-WHEEL_DELTA,0);
				break;
			default:
				printf("Unknown Event!\n");
				return false;
		}
	}
	return true;
}

