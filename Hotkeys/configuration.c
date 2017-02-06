#include "configuration.h"

void free_split(char** ptr)
{
	if (*ptr)
		free(*ptr);
	free(ptr);
}

char** split(const char* str, const char* delimiter)
{
	int str_len = strlen(str);
	int del_len = strlen(delimiter);

	char* str_cpy = malloc(sizeof(char)*(str_len+1));
	strcpy_s(str_cpy, sizeof(char)*(str_len+1), str);

	char** s = malloc(sizeof(char*));
	*s = NULL;
	
	for(int i = 0 ; str_cpy; i++)
	{
		s = realloc(s,sizeof(char*)*(i+2));
		s[i] = str_cpy;
		s[i+1] = NULL;

		char* next = strstr(str_cpy, delimiter);
		if (next) {
			*next = 0;
			str_cpy = next + del_len;
		}
		else str_cpy = NULL;
	}
	return s;
}

int loadConfiguration(char * filename)
{
	FILE * file;
	char * in = (char *)malloc(sizeof(char));
	int count;
	int ferror = 0;
	ferror = fopen_s(&file, filename, "r");
	if (ferror)
	{
		ferror = fopen_s(&file, CONFIGURATION_FILE, "r");
		if (ferror) return ferror;
	}
	for (count = 0; 1; count++)
	{
		in[count] = fgetc(file);
		if (feof(file)) break;
		in = realloc(in, sizeof(char) * (count+2));
	}
	in[count] = 0;
	fclose(file);
	parseConfiguration(in);
	return 0;
}

int parseConfiguration(char * config)
{
	hotkeys_config.count = 0;
	char** lines = split(config,"\n");
	free(config);
	for (int i = 0; lines[i]; i++)
		parseLine(lines[i]);
	free_split(lines);
	return 0;
}

int parseLine(char * line)
{
	int len = strlen(line);
	if(len<3) return 0;
	char** args = split(line, "\t");
	for (int i = 0; args[i]; i++)
	{
		switch (i)
		{
			case 0:
			{
				hotkeys_config.evt = realloc(hotkeys_config.evt, sizeof(HOTKEY_EVT) * (++hotkeys_config.count));
				hotkeys_config.evt[hotkeys_config.count - 1].code = atoi(args[i]);
				break;
			}
			case 1:
			{
				int action_len = strlen(args[i]);
				char * action = malloc(sizeof(char)*(action_len + 1));
				strcpy_s(action, sizeof(char)*(action_len + 1), args[i]);
				hotkeys_config.evt[hotkeys_config.count - 1].action = action;
				break;
			}
			default:
			{
				int old_len = strlen(hotkeys_config.evt[hotkeys_config.count - 1].action);
				int action_len = strlen(args[i]);
				hotkeys_config.evt[hotkeys_config.count - 1].action = realloc(hotkeys_config.evt[hotkeys_config.count - 1].action, sizeof(char)*(old_len + action_len + 1));
				hotkeys_config.evt[hotkeys_config.count - 1].action[old_len] = "\t";
				strcpy_s(hotkeys_config.evt[hotkeys_config.count - 1].action + old_len + 1, sizeof(char)*(action_len + 1), args[i]);
				break;
			}
		}
	}
	free_split(args);
	return 0;
}

void cleanupConfiguration()
{
	for (int i = 0; i < hotkeys_config.count; i++)
		free(hotkeys_config.evt[i].action);
	free(hotkeys_config.evt);
}
