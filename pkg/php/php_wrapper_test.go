package php

import (
	"github.com/stretchr/testify/require"
	"testing"
)

func TestPhpInitAndShutdown(t *testing.T) {
	require.NoError(t, phpModuleInit())
	phpThreadInit()
	require.NoError(t, phpModuleShutdown())
}
