#pragma once

// UNIX
#ifdef __unix__

#include <unistd.h>
#define ISATTY isatty

// WINDOWS
#else

#include <io.h>
#define ISATTY _isatty

#endif


/**
	Builds cross-platform layer for
	some functionality
*/
struct Platform {
	static bool isInteractive() {
		return ISATTY(0);
	}
};