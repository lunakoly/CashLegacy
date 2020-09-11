// Copyright (C) 2020 luna_koly
//
// Windows-specific task for declaring a
// general process task.


#pragma once


#include "../tasks.hpp"

#include <vector>

#include "tasks.hpp"


/**
 * Windows-specific task for declaring a
 * general process task.
 */
class WindowsProcess : public DuplexTask<WindowsInputTask, WindowsOutputTask> {
public:
	explicit WindowsProcess(const std::vector<std::string> & arguments)
		: arguments(arguments) {}

    virtual std::optional<std::string> then(WindowsOutputTask *) override;

	virtual std::optional<std::string> configure(WindowsInputTask *) override;

    virtual std::optional<std::string> launch() override;

    virtual std::optional<std::string> wait() override;

private:
    /**
	 * Stored here to allow wait to
	 * call WaitForSingleObject() further.
	 */
	PROCESS_INFORMATION process_info;
	/**
	 * Stores the command representation.
	 */
	std::vector<std::string> arguments;
};
