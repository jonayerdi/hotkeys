#pragma once

#include <stdio.h>
#include <Windows.h>
#include <WinUser.h>

#define CONFIGURATION_FILE "hotkeys.ini"

typedef struct HOTKEY_EVT_S
{
	DWORD code;
	char* action;
} HOTKEY_EVT;

typedef struct HOTKEYS_S
{
	HOTKEY_EVT* evt;
	DWORD count;
} HOTKEYS;

HOTKEYS hotkeys_config;

void free_split(char** ptr);
char** split(const char* str, const char* delimiter);

int loadConfiguration(char * file);
int parseConfiguration(char * config);
int parseLine(char * line);
void cleanupConfiguration();
