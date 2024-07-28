#pragma once

#include <stdlib.h>

int limos_php_module_init(int threadCount);
int limos_php_thread_init();
int limos_php_execute(char* script, size_t valuesLen, char** keys, char** values);
int limos_php_module_shutdown();