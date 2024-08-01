#include "php_wrapper.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sapi/embed/php_embed.h>
#include <libgen.h>

const char HARDCODED_INI[] =
    "html_errors=0\n"
    "register_argc_argv=1\n"
    "implicit_flush=1\n"
    "output_buffering=0\n"
    "max_execution_time=0\n"
    "max_input_time=-1\n\0";

int limos_php_module_init(int threadCount) {
    fprintf(stderr, "Initializing the PHP module. Thread count: %d\n", threadCount);
    if (!php_tsrm_startup_ex(threadCount)) return -1;
    sapi_startup(&php_embed_module);

    php_embed_module.ini_entries = HARDCODED_INI;
     if (php_embed_module.startup(&php_embed_module) == FAILURE) {
        sapi_shutdown();
        tsrm_shutdown();
        return -1;
    }

    return 0;
}

void limos_php_thread_init() {
    /* initial resource fetch */
    (void)ts_resource(0);
}

int limos_php_execute(char* script, size_t valuesLen, char** keys, char** values) {
    if (php_request_startup() == FAILURE) return -1;

    FILE* fp = fopen(script, "r");
    if (fp == NULL) {
        fprintf(stderr, "Could not file PHP file to execute: %s\n", script);
        php_request_shutdown((void*) 0);
        return -1;
    }

    zend_file_handle file_handle;
    zend_stream_init_fp(&file_handle, fp, basename(script));
    php_execute_script(&file_handle);

    int exitStatus = EG(exit_status);
    php_request_shutdown((void*) 0);

    return 0;
}

int limos_php_module_shutdown() {
    sapi_shutdown();
    tsrm_shutdown();
    return 0;
}