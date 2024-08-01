package php

import (
	"github.com/stretchr/testify/require"
	"testing"
)

func TestPhpInitAndShutdown(t *testing.T) {
	require.NoError(t, PhpModuleInit())
	require.NoError(t, PhpModuleShutdown())
}
