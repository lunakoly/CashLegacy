#pragma once

#include <string>

// FILENAME_MAX
#include <stdio.h>

// UNIX
#ifdef __unix__

#include <unistd.h>
#define ISATTY isatty
#define GETCWD getcwd

// WINDOWS
#else

#include <io.h>
#define ISATTY _isatty
#include <direct.h>
#define GETCWD _getcwd

#endif


/**
	Builds cross-platform layer for
	some functionality
*/
struct Platform {
	/**
		Returns true if stdin is connected
		to an interactive input device (terminal)
	*/
	static bool isInteractive() {
		return ISATTY(0);
	}

	/**
		Returns the current working directory path
	*/
	static std::string getCurrentDirectory() {
		char buffer[FILENAME_MAX];
		GETCWD(buffer, FILENAME_MAX);
		return std::string(buffer);
	}
};