#include "handle_cmd.h"
#include "cKeyMouseWrapper.h"
#include <iostream>
#include <sstream>
#include <regex>
using namespace std;
CKeyMouseWrapper rb = CKeyMouseWrapper();

const int st = 500;
//×Ö·û´®·Ö¸îº¯Êý
std::vector<std::string> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//À©Õ¹×Ö·û´®ÒÔ·½±ã²Ù×÷
	size_t size = str.size();

	for (size_t i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

std::vector<std::wstring> splitWString(const std::wstring& str, wchar_t delimiter) {
	std::vector<std::wstring> parts;
	std::wstringstream wss(str);
	std::wstring temp;
	while (std::getline(wss, temp, delimiter))
		parts.push_back(temp);
	return parts;
}
string myGetEnv(const char * var)
{

    string rt;
    char* pValue;
    size_t len;
    errno_t err = _dupenv_s(&pValue, &len, var);
    if (err) return "";
    if (pValue)
    {
        rt = string(pValue);
    }
    free(pValue);
    return rt;
}



void handleCmd(std::wstring cmd)
{


	/// Execute a command string. Supported mini-language:
/// - groups separated by `/`
/// - `a-x` : Alt+<x>
/// - 'c--' : Ctrl down 'c-+' : Ctrl up
/// - `m-1` / `m-2` / `m-3` : mouse left/right/middle click
/// - `-` : backspace
/// - `>` : enter
/// - '^' : up key
/// - `@` : activate AutoCAD (window helper)
/// - `slp-N` : sleep N ms
/// - 'tab' : press tab key
/// - `window,title,x,y,cmd_str` : activate window and move to position x y is offset from left top corner, then type cmd_str
/// - otherwise: type the literal substring

// get all groups separated by `/` 
	cout << "in handleCmd()" << endl;
	std::wregex group_regex(L"([^/]+)");
	auto groups_begin = std::wsregex_iterator(cmd.begin(), cmd.end(), group_regex);
	auto groups_end = std::wsregex_iterator();
	for (std::wsregex_iterator i = groups_begin; i != groups_end; ++i) {
		std::wsmatch match = *i;
		std::wstring group = match.str(1);
		// process each group
		size_t pos = 0;
		if (group == L"a-" || group.find(L"a-") == 0)
		{
			// Alt+<x>
			std::vector<std::wstring> parts = splitWString(group, L'-');
			if (parts.size() == 2) {
				wchar_t key = parts[1][0];
				rb.PressAltAndKey(key);
				//rb.PressKey(VK_MENU, true);
				//Sleep(50);
				//rb.TypeKey(key);
				//Sleep(50);
				//rb.PressKey(VK_MENU, false);
			}
		}
		else if (group == L"c--")
		{
			// Ctrl down
			rb.PressKey(VK_CONTROL, true);
			Sleep(st);
			// rb.TypeKey(VK_TAB);
			// Sleep(50);
			// rb.PressKey(VK_CONTROL, false);

		}
		else if (group == L"c-+")
		{
			// Ctrl up
			Sleep(st);
			rb.PressKey(VK_CONTROL, false);
		}
		else if (group == L"tab")
		{
			// tab key
			rb.TypeKey(VK_TAB);
			Sleep(st);
		}
		else if (group == L"d-1")
		{
      // PgDn
			rb.TypeKey(0x22);
			Sleep(st);
		}
		else if (group == L"m-1")
		{
			// mouse click
			rb.clickButton(1);
		}
		else if (group == L"m-2")
		{
			// mouse click
			rb.clickButton(2);
		}
		else if (group == L"m-3")
		{
			// mouse click
			rb.clickButton(3);
		}
		else if (group == L"-")
		{
			// backspace
			rb.TypeKey(VK_BACK);
		}
		else if (group == L">")
		{
			// enter
			rb.TypeKey(VK_RETURN);
		}
		else if (group == L"^")
		{
			// enter
			rb.TypeKey(VK_UP);
		}
		else if (group == L"@")
		{
			// activate AutoCAD window
			//HWND acadHwnd = (HWND)getRegKeyDword("acad\\acad_hook", "acad_hwnd");
			//if (acadHwnd) {
				//SetForegroundWindow(acadHwnd);
			//}
		}
		else if (group.find(L"slp-") == 0)
		{
			// sleep N ms
			std::vector<std::wstring> parts = splitWString(group, L'-');
			if (parts.size() == 2)
			{
				try {
					int sleep_time = std::stoi(parts[1]);
					Sleep(sleep_time);
				}
				catch (...) {}
			}
		}
		else
		{

			// type the literal substring
			rb.TypeString(group.c_str());
		}
	}

}
