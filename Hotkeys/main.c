#include "main.h"
#include "resource.h"

void SignalHandler(int signal)
{
	Shell_NotifyIcon(NIM_DELETE, &tray_icon);
	cleanupConfiguration();
}

BOOL CheckOneInstance()
{
	HANDLE m_hStartEvent = CreateEvent(NULL, TRUE, FALSE, HOTKEYS_EVENT);
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		CloseHandle(m_hStartEvent);
		m_hStartEvent = NULL;
		// already exists
		return FALSE;
	}
	// the only instance, start in a usual way
	return TRUE;
}

int _stdcall WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	int retcode = 0;

	if (!CheckOneInstance())
		return 0;

	signal(SIGTERM, SignalHandler);
	create_window(hInstance);
	set_tray_icon(hInstance);
	if (!(retcode = loadConfiguration(lpCmdLine)))
	{
		HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHandle, NULL, 0);
#ifdef HOTKEYS_LOGGING
		logString("Hooked to keyboard events");
#endif
		MSG msg;
		while (1) {
			GetMessage(&msg, NULL, 0, 0);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		UnhookWindowsHookEx(hook);
#ifdef HOTKEYS_LOGGING
		logString("Unhooked from keyboard events");
#endif
	}
	cleanupConfiguration();
	ReleaseMutex(hMutex);
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

void create_window(HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	/*wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);*/
	wc.lpszMenuName = NULL;
	wc.lpszClassName = window_class;
	//wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	hWindow = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		window_class,
		"Hotkeys",
		WS_OVERLAPPEDWINDOW,
		0, 0, 0, 0,
		NULL, NULL, hInstance, NULL);

	if (hWindow == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	//ShowWindow(hWindow, SW_SHOW);
	//UpdateWindow(hWindow);
}

void set_tray_icon(HINSTANCE hInstance)
{
	ZeroMemory(&tray_icon, sizeof(NOTIFYICONDATA));
	tray_icon.hWnd = hWindow;
	tray_icon.uFlags = NIF_ICON;
	tray_icon.hIcon = LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
	tray_icon.cbSize = sizeof(tray_icon);
	Shell_NotifyIcon(NIM_ADD, &tray_icon);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		Shell_NotifyIcon(NIM_DELETE, &tray_icon);
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
