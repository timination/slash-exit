#include "stdafx.h"
#include<Windows.h>
#include <iostream>



HHOOK hHook = NULL;
bool consec = false;
char str[] = "/exit";
int inCount = 0;

LRESULT CALLBACK MyLowLevelHook(int nCode, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT* hs = (KBDLLHOOKSTRUCT*)lParam;
	if (nCode <0)
		return CallNextHookEx(hHook, nCode, wParam, lParam);

	switch (hs->vkCode)
	{
	case VK_OEM_2: // forward-slash
		if (inCount == 0) consec = true; inCount++;
		break;

	case 0x45: // E
		if (inCount == 1) consec = true; inCount++;
		break;

	case 0x58: // X
		if (inCount == 2) consec = true; inCount++;
		break;

	case 0x49: // I
		if (inCount == 3) consec = true; inCount++;
		break;

	case 0x54: // T
		if (inCount == 4) consec = true; inCount++;
		break;

	default: // if any other key is pressed or keys are not pressed in sequence, reset counts and consec bool
		consec = false;
		inCount = 0;
		break;
	}
	// if count reaches 5, all keys have been pressed in order. 
	if(inCount == 5)
	{
		system("shutdown -s -t 60 -c \"You typed /exit\"");
		inCount = 0;
	}
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

int main()
{
	FreeConsole();

	MSG msg;
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, MyLowLevelHook, NULL, NULL);

	while (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)>0)
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(hHook);
	return 0;
}