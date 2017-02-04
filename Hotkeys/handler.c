#include "handler.h"

void logString(char * data)
{
	FILE * file;

	time_t rawtime;
	struct tm timeinfo;
	char timeString[26];
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);

	fopen_s(&file, "hotkeys.log", "a");
	fputc('[', file);
	asctime_s(timeString, 26, &timeinfo);
	timeString[strlen(timeString) - 1] = ']';
	fputs(timeString, file);
	fputc(' ', file);
	fputs(data, file);
	fputc('\n', file);
	fclose(file);
}

int action(char* command)
{
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (CreateProcessA(NULL, command, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	ExitThread(0);
}

void handleKeyboardEvent(unsigned __int64 type, KBDLLHOOKSTRUCT * eventData)
{
	switch (type)
	{
	case WM_KEYDOWN:
		for (int i = 0 ; i < hotkeys_config.count ; i++)
			if (hotkeys_config.evt[i].code == eventData->vkCode)
			{
				CreateThread(
					NULL,								// default security attributes
					0,									// use default stack size  
					action,								// thread function name
					hotkeys_config.evt[i].action,       // argument to thread function 
					0,									// use default creation flags 
					NULL);								// return 

				int action_len = strlen(hotkeys_config.evt[i].action);
				char * message = malloc(sizeof(char)*(action_len + 80));
				sprintf_s(message, action_len + 80, "KEYDOWN=%d\tACTION=%s", eventData->vkCode, hotkeys_config.evt[i].action);
				logString(message);
				free(message);
			}
		break;
	case WM_KEYUP:
		break;
	case WM_SYSKEYDOWN:
		break;
	case WM_SYSKEYUP:
		break;
	default:
		break;
	}
}
