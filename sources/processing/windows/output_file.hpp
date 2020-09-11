// Copyright (C) 2020 luna_koly
//
// Unix-specific task for declaring a
// file output task.


#pragma once


#include "../tasks.hpp"

#include <windows.h>

#include "tasks.hpp"


/**
 * Unix-specific task for declaring a
 * file output task.
 */
class WindowsOutputFile : public WindowsOutputTask {
public:
    WindowsOutputFile(const std::string & filename)
        : filename(filename) {}

    virtual std::optional<std::string> configure(WindowsInputTask *) override;

    virtual std::optional<std::string> launch() override;

    virtual std::optional<std::string> wait() override;

private:
	/**
	 * The file to write.
	 */
	std::string filename;
};

