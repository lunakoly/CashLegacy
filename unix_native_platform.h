#pragma once

#include <string>

// FILENAME_MAX
#include <stdio.h>
// isatty, getcwd, gethostname
#include <unistd.h>
// getpwuid, getuid
#include <pwd.h>
// HOST_NAME_MAX
#include <limits.h>


/**
	Platform-specific implementation of functions
	needed to be cross-platform
*/
struct NativePlatform {
	/**
		Returns true if stdin is connected
		to an interactive input device (terminal)
	*/
	static bool isInteractive() {
		return isatty(0);
	}

	/**
		Returns the current working directory path
	*/
	static std::string getCurrentDirectory() {
		char buffer[FILENAME_MAX];
		getcwd(buffer, FILENAME_MAX);
		return std::string(buffer);
	}

	/**
		Returns user name
	*/
	static std::string getUserName() {
		return std::string(getpwuid(getuid())->pw_name);
	}

	/**
		Returns host name
	*/
	static std::string getHostName() {
		char buffer[HOST_NAME_MAX];
		size_t length = HOST_NAME_MAX;
		gethostname(buffer, length);
		return std::string(buffer);
	}
};