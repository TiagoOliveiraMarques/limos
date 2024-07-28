package main

import (
	"github.com/TiagoOliveiraMarques/limos/pkg/php"
	"log/slog"
)

func main() {
	err := php.PhpModuleInit()
	if err != nil {
		panic(err)
	}

	slog.Info("Done")
}
