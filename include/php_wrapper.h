#ifndef LIMOS_PHP_WRAPPER_H_INCLUDED
#define LIMOS_PHP_WRAPPER_H_INCLUDED

#include <stdlib.h>

int limos_php_execute(const char* script, size_t len, char** keys, char** values);

#endif