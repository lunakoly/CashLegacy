#pragma once

#include <string>

#ifdef __unix__
#include "unix_native_platform.h"
#else
#include "windows_native_platform.h"
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
		return NativePlatform::isInteractive();
	}

	/**
		Returns the current working directory path
	*/
	static std::string getCurrentDirectory() {
		return NativePlatform::getCurrentDirectory();
	}

	/**
		Returns user name
	*/
	static std::string getUserName() {
		return NativePlatform::getUserName();
	}

	/**
		Returns host name
	*/
	static std::string getHostName() {
		return NativePlatform::getHostName();
	}
};