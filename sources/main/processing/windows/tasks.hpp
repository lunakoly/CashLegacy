// Copyright (C) 2020 luna_koly
//
// Windows-specific tasks declarations.


#pragma once


#include "../tasks.hpp"

#include <windows.h>


struct WindowsInputTask;


/**
 * OutputTask with Windows-specific data.
 */
struct WindowsOutputTask : public OutputTask<WindowsInputTask> {
    /**
	 * Handle that acts as stdin for this task.
	 * For the sake of simplicity I leave a member here,
     * but it's only used within a Process instance, so we
     * may want to replace it with accessor methods,
     * but I'm too lazy to do it now.
	 */
	HANDLE input = NULL;
};


/**
 * InputTask with Windows-specific data.
 */
struct WindowsInputTask : public InputTask<WindowsOutputTask> {
	/**
	 * Handle that acts as stdout for this task.
	 * For the sake of simplicity I leave a member here,
     * but it's only used within a Process instance, so we
     * may want to replace it with accessor methods,
     * but I'm too lazy to do it now.
	 */
	HANDLE output = NULL;
};
