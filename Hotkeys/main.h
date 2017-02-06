#pragma once

#include <signal.h> 
#include "handler.h"

#define HOTKEYS_EVENT "Hotkeys.0"

HANDLE hMutex;
HWND hWindow;
const char window_class[] = "hotkeysWindowClass";
NOTIFYICONDATA tray_icon;

BOOL CheckOneInstance();

LRESULT CALLBACK KeyboardHandle(
	_In_ int    code,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);

void create_window(HINSTANCE hInstance);
void set_tray_icon(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
