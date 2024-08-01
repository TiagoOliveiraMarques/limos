package php

// #cgo CFLAGS: -I /usr/local/include/php
// #cgo CFLAGS: -I /usr/local/include/php/main
// #cgo CFLAGS: -I /usr/local/include/php/TSRM
// #cgo CFLAGS: -I /usr/local/include/php/Zend
// #cgo CFLAGS: -I /usr/local/include/php/ext
// #cgo CFLAGS: -I/usr/local/include/php/ext/date/lib
// #cgo CFLAGS: -Wno-implicit-function-declaration
// #cgo CFLAGS: -Wno-int-to-pointer-cast
// #cgo LDFLAGS: -lphp
// #include "php_wrapper.h"
import "C"
import (
	"errors"
	"runtime"
	"unsafe"
)

func phpModuleInit() error {
	threadCount := max(1, runtime.NumCPU()-1)
	status := C.limos_php_module_init(C.int(threadCount))
	if status != 0 {
		return errors.New("Failed to initialize PHP module")
	}

	return nil
}

func phpModuleShutdown() error {
	if status := C.limos_php_module_shutdown(); status != 0 {
		return errors.New("Failed to shutdown PHP module")
	}

	return nil
}

func phpThreadInit() {
	C.limos_php_thread_init()
}

func phpExecuteScript(script string, values map[string]string) error {
	cscript := C.CString(script)
	defer C.free(unsafe.Pointer(cscript))

	if status := C.limos_php_execute(cscript, C.size_t(0), nil, nil); status != 0 {
		return errors.New("Failed to execute PHP script")
	}

	return nil
}
