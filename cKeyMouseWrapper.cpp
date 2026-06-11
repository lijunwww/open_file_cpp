//#include "stdafx.h"
#include "cKeyMouseWrapper.h"

#include <atlconv.h>  



void CKeyMouseWrapper::PressKey(wchar_t key, bool down)
{
	INPUT in[1]={0};
	in[0].type = INPUT_KEYBOARD;
	in[0].ki.wVk = key;
	in[0].ki.dwFlags = !down*KEYEVENTF_KEYUP;
	SendInput(1, in, sizeof(INPUT));
}
/*
**	功能：
**		模拟按键动作
例子： 
TypeKey(L'A', 30)
TypeKey(L'A')
*/
void CKeyMouseWrapper::TypeKey(wchar_t key, int keystroke_time)
{
    
	this->PressKey(key);
	::Sleep(keystroke_time);
	this->PressKey(key, false);
	return;
}
/*	功能： Ctrl + Key
*	如：	PressCtrAndKey(L'A', 100)
*	如：	PressCtrAndKey(L'A')
*/
void CKeyMouseWrapper::PressCtrlAndKey(wchar_t key, int keystroke_time)
{
	this->PressKey(VK_CONTROL);
	this->TypeKey(key, keystroke_time);
	this->PressKey(VK_CONTROL, false);

}
void CKeyMouseWrapper::PressAltAndKey(wchar_t key, int keystroke_time)
{
	this->PressKey(VK_MENU);
	this->TypeKey(key, keystroke_time);
	this->PressKey(VK_MENU, false);

}

void CKeyMouseWrapper::SendUnicode(wchar_t data)
{
	INPUT input[2];
	memset(input, 0, 2 * sizeof(INPUT));

	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0;
	input[0].ki.wScan = data;
	input[0].ki.dwFlags = 0x4;//KEYEVENTF_UNICODE;

	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0;
	input[1].ki.wScan = data;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP | 0x4;//KEYEVENTF_UNICODE;

	SendInput(2, input, sizeof(INPUT));
}
//为方便使用，下面这个函数包装了前两个函数。
void CKeyMouseWrapper::TypeString(const wchar_t * msgin, int keystroke_time)
	//void SendKeys(CString msg)
{
	short vk;
	BOOL shift;
	wchar_t *msg=const_cast< wchar_t* >(msgin);
	USES_CONVERSION;
	//wchar_t* data = T2W(msg.GetBuffer(0));
	wchar_t* data = T2W(msg);
	size_t len = wcslen(data);

	for(int i=0;i<len;i++)
	{
		if (data[i]>=0 && data[i]<256) //ascii字符
		{
			vk = VkKeyScanW(data[i]);

			if (vk == -1)
			{
				SendUnicode(data[i]);
			}
			else
			{
				if (vk < 0)
				{
					vk = ~vk + 0x1;
				}

				shift = vk >> 8 & 0x1;

				if (GetKeyState(VK_CAPITAL) & 0x1)
				{
					if (data[i]>='a' && data[i]<='z' || data[i]>='A' && data[i]<='Z')
					{
						shift = !shift;
					}
				}

				//SendAscii(vk & 0xFF, shift);

				if(shift)
				{
					this->PressKey(VK_SHIFT);
				}
				this->TypeKey(vk & 0xFF);
				if(shift)
				{
					this->PressKey(VK_SHIFT, false);
				}


			}
		}
		else //unicode字符
		{
			SendUnicode(data[i]);
		}
		::Sleep(keystroke_time);
	}
	return;

}

void CKeyMouseWrapper::screenToMouse(POINT &scrPt)
{

	int cx_screen = ::GetSystemMetrics(SM_CXSCREEN);  //屏幕 宽
	int cy_screen = ::GetSystemMetrics(SM_CYSCREEN);  //     高

	scrPt.x = 65535 * scrPt.x / cx_screen;  //转换后的 x
	scrPt.y= 65535 * scrPt.y / cy_screen;  //         y
	return;
}

void CKeyMouseWrapper::MoveToPoint(long x, long y, int duration, int tick)
{



	POINT curPt, pt;
	::GetCursorPos(&curPt);
	this->screenToMouse(curPt);
	pt.x = x;
	pt.y = y;
	this->screenToMouse(pt);


	LONG num_steps = (duration / tick) + 1;
	LONG x_step_size = (pt.x - curPt.x) / num_steps;
	LONG y_step_size = (pt.y - curPt.y) / num_steps;

	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.mouseData = 0;
	input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;   //MOUSEEVENTF_ABSOLUTE 代表决对位置  MOUSEEVENTF_MOVE代表移动事件
	input.mi.time = 0;
	input.mi.dwExtraInfo = 0;

	int step = 0;
	while(step < num_steps)
	{
		curPt.x += x_step_size;
		curPt.y += y_step_size;
		input.mi.dx = curPt.x;
		input.mi.dy = curPt.y ;
		SendInput(1,&input,sizeof(INPUT));
		step += 1;
		::Sleep(tick);


	}


	return;

}
//void CKeyMouseWrapper::pressButton(BOOL down, char* button)
void CKeyMouseWrapper::pressButton(bool down, int button)
{
	// 0: left 1: right 2: middle
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.mouseData = 0;
	if(down)
	{
		if(button == 1)
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		else if(button == 2)
		input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
		else if(button == 3)
		input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
	}

	else
	{
		if(button == 1)
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		else if(button == 2)
		input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
		else if(button == 3)
		input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
	}


	input.mi.time = 0;
	input.mi.dwExtraInfo = 0;
	SendInput(1,&input,sizeof(INPUT));

	return;
}

void CKeyMouseWrapper::clickButton(int button, int click_time )
{
	this->pressButton(TRUE, button);
	::Sleep(click_time);
	this->pressButton(FALSE, button);
	return;
}
void CKeyMouseWrapper::GetCurMousePosition(POINT &curPt){

	HWND curHwnd = GetForegroundWindow(); //获取句柄
		DWORD selfThreadID = GetCurrentThreadId();
		DWORD curThreadID = GetWindowThreadProcessId(curHwnd, NULL);
		if (::AttachThreadInput(selfThreadID, curThreadID, TRUE))
		{
			::GetCursorPos(&curPt);

		}
		else
		{
			curPt.x = 600;
			curPt.y = 380;
		}
		AttachThreadInput(selfThreadID, curThreadID, FALSE);
		return;



}
void CKeyMouseWrapper::DoubleClickButton(int button, int click_time, int time_between_clicks)
{
	this->clickButton(button, click_time);
	::Sleep(time_between_clicks);
	this->clickButton(button, click_time);

	return;
}