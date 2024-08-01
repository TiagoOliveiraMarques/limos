package php

import (
	"github.com/stretchr/testify/require"
	"testing"
)

func TestPhpExecutionWithoutValues(t *testing.T) {
	script := "/app/samples/hello_world.php"
	require.NoError(t, phpExecuteScript(script, make(map[string]string)))
}
