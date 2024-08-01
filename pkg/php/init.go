package php

func init() {
	// TODO: There is a bug somewhere and we cannot initialize and shutdown multiple times in the same process.
	// So what we do today is to call this in an init function.
	// Once the API gets stable we can convert this into a singleton and have proper init and shutdown.
	if err := phpModuleInit(); err != nil {
		panic(err)
	}
}
