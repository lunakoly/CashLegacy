#pragma once

#include <string>

// FILENAME_MAX
#include <stdio.h>
// UNLEN
#include <Lmcons.h>
// _isatty
#include <io.h>
// _getcwd
#include <direct.h>
// GetUserName
#include <windows.h>


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
		return _isatty(0);
	}

	/**
		Returns the current working directory path
	*/
	static std::string getCurrentDirectory() {
		char buffer[FILENAME_MAX];
		_getcwd(buffer, FILENAME_MAX);
		return std::string(buffer);
	}

	/**
		Returns user name
	*/
	static std::string getUserName() {
		char buffer[UNLEN + 1];
		DWORD length = UNLEN + 1;
		GetUserName(buffer, &length);
		return std::string(buffer);
	}

	/**
		Returns host name
	*/
	static std::string getHostName() {
		char buffer[UNLEN + 1];
		DWORD length = UNLEN + 1;
		GetComputerName(buffer, &length);
		return std::string(buffer);
	}
};