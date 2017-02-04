#pragma once

#include "handler.h"

LRESULT CALLBACK KeyboardHandle(
	_In_ int    code,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);
