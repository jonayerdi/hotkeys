#include "main.h"

int _stdcall WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	int retcode = 0;
	if (!(retcode = loadConfiguration(lpCmdLine)))
	{
		HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHandle, NULL, 0);
		logString("Hooked to keyboard events");
		MSG msg;
		while (!GetMessage(&msg, NULL, NULL, NULL)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		UnhookWindowsHookEx(hook);
		logString("Unhooked from keyboard events");
	}
	cleanupConfiguration();
	return retcode;
}

LRESULT CALLBACK KeyboardHandle(
	_In_ int    code,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam)
{
	handleKeyboardEvent(wParam, (KBDLLHOOKSTRUCT *)lParam);
	return CallNextHookEx(NULL, code, wParam, lParam);
}
