//#include <atlconv.h>  
#include <windows.h>


class CKeyMouseWrapper
{
public:
/*
	功能：	按下或松开一个键

*/
void PressKey(wchar_t data, bool down = true);
void TypeKey(wchar_t data, int keystroke_time=0);
void TypeString(const wchar_t * msg , int keystroke_time=0);
	void SendUnicode(wchar_t data);
	//void SendAscii(wchar_t data,BOOL shift);
	void PressCtrlAndKey(wchar_t key, int keystroke_time = 0);
	void PressAltAndKey(wchar_t key, int keystroke_time = 0);
	void MoveToPoint(long x, long y, int duration = 50, int tick = 100);
	void pressButton(bool down=true, int button=1 );
	void clickButton(int button=1, int click_time=100);
	void GetCurMousePosition(POINT &curPt);
	void DoubleClickButton(int button=1, int click_time=0, int time_between_clicks = 0);
private:
	void screenToMouse(POINT &scrPt);
};
