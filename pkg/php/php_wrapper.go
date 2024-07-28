package php

// #cgo CFLAGS: -I /usr/local/include/php
// #cgo CFLAGS: -I /usr/local/include/php/main
// #cgo CFLAGS: -I /usr/local/include/php/TSRM
// #cgo CFLAGS: -I /usr/local/include/php/Zend
// #cgo CFLAGS: -I /usr/local/include/php/ext
// #cgo CFLAGS: -I/usr/local/include/php/ext/date/lib
// #cgo LDFLAGS: -lphp
// #include "php_wrapper.h"
import "C"
import (
	"errors"
	"runtime"
)

func PhpModuleInit() error {
	threadCount := max(1, runtime.NumCPU()-1)
	status := C.limos_php_module_init(C.int(threadCount))
	if status != 0 {
		return errors.New("Failed to initialize PHP module")
	}

	return nil
}
