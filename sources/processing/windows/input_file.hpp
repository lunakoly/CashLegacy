// Copyright (C) 2020 luna_koly
//
// Windows-specific task for declaring a
// file input task.


#pragma once


#include "../tasks.hpp"

#include <windows.h>

#include "tasks.hpp"


/**
 * Windows-specific task for declaring a
 * file input task.
 */
class WindowsInputFile : public WindowsInputTask {
public:
    WindowsInputFile(const std::string & filename)
        : filename(filename) {}

    virtual std::optional<std::string> then(WindowsOutputTask *) override;

    virtual std::optional<std::string> launch() override;

    virtual std::optional<std::string> wait() override;

private:
	/**
	 * The file to be read.
	 */
	std::string filename;
};
