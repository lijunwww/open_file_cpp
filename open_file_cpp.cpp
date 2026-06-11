// open_file_cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <atlstr.h>
#include <iostream>
#include <sstream>
#include <string>
#include <regex>
#include <vector>
#include <cwchar>
#include <cwctype>
using namespace std;

#include "handle_cmd.h"


HWND wnd_found;

struct WF {
	string txtInTitle;
	HWND hwnd;
};




BOOL CALLBACK get_window_by_title_callback(HWND hwnd, LPARAM lparam) {
	char title[256];


	if (GetWindowTextA(hwnd, title, sizeof(title)))
	{
		string windowTitle(title);
		if (windowTitle.find(((WF*)lparam)->txtInTitle) != std::string::npos)
		{
			cout << "found" << endl;
			cout << "window title is = " << windowTitle.c_str() << endl;
			//((WF*)lparam)->hwnd = hwnd;
			wnd_found = hwnd;
			return false;
		}
	}
	return true;
}
void findWindowByTitle(string& txt)
{
	WF wf = {};
	wf.txtInTitle = txt;
	EnumWindows(get_window_by_title_callback, (LPARAM)(&wf));
}



int main(int argc, char* argv[])
{
	printf("argc = %d\n", argc);
	for (int ndx = 0; ndx != argc; ++ndx)
		printf("argv[%d] --> %s\n", ndx, argv[ndx]);
	// open_file_cpp.exe file_path  post_cmd
	string file_path;
	wstring w_cmd;
	if (argc == 2)
	{
		file_path = string(argv[1]);
	}
	else if (argc == 3)
	{
		file_path = string(argv[1]);
		CA2CT ca_to_ct(argv[2]);
		w_cmd = wstring(ca_to_ct);
	}
	else
	{
		return 0;
	}
	cout << file_path.c_str() << endl;
	wcout << w_cmd.c_str() << endl;



	HWND cmdLineWnd = GetConsoleWindow();
	if (cmdLineWnd != NULL)
	{
		ShowWindow(cmdLineWnd, SW_HIDE);
	}

	string cmd = "explorer.exe ";
	cmd += file_path;
	//cmd += " D:\\bb\\现代实用机床设计手册(上册).pdf";
	WinExec(cmd.c_str(), SW_HIDE);
	//Sleep(1500);
	//string tar = "现代实用机床设计手册";
	string tar;
	auto pos = file_path.find_last_of('\\');
	tar = file_path.substr(pos + 1);
	//auto pos1 = tar.find_last_of('.');
	//tar = tar.substr(0, pos1);
	//cout << "tar = " << tar.c_str() << endl;

	Sleep(800);
	int cnt = 0;
	while (true)
	{
		findWindowByTitle(tar);
		if (wnd_found)
		{
			//ShowWindow(wnd_found, SW_SHOWNORMAL);
			ShowWindow(wnd_found, SW_SHOWMAXIMIZED);
			SetForegroundWindow(wnd_found);
			Sleep(300);
			break;
		}
		else {
			Sleep(300);
			cnt++;
		}
		if (cnt++ > 30) { return 0; }
	}
	handleCmd(w_cmd);

	//rb.PressAltAndKey(VK_TAB);
	//Sleep(st);

	//rb.PressAltAndKey(L'V');
	//Sleep(st);

	//rb.TypeKey(L'G');
	//Sleep(st);

	//rb.TypeKey(L'P');
	//Sleep(st);
	//rb.TypeString(L"55");
	//rb.TypeKey(VK_RETURN);


	//

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
