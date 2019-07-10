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
namespace Platform {
	/**
		Returns true if stdin is connected
		to an interactive input device (terminal)
	*/
	bool isInteractive() {
		return NativePlatform::isInteractive();
	}

	/**
		Returns the current working directory path
	*/
	std::string getCurrentDirectory() {
		return NativePlatform::getCurrentDirectory();
	}

	/**
		Returns user name
	*/
	std::string getUserName() {
		return NativePlatform::getUserName();
	}

	/**
		Returns host name
	*/
	std::string getHostName() {
		return NativePlatform::getHostName();
	}
};