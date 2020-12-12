// Copyright (C) 2020 luna_koly
//
// Unix-specific task for declaring a
// general process task.


#pragma once


#include "../tasks.hpp"

#include <vector>

#include "tasks.hpp"


/**
 * Unix-specific task for declaring a
 * general process task.
 */
class UnixProcess : public DuplexTask<UnixInputTask, UnixOutputTask> {
public:
	explicit UnixProcess(const std::vector<std::string> & arguments)
		: arguments(arguments) {}

	virtual std::optional<std::string> then(UnixOutputTask *) override;

	virtual std::optional<std::string> configure(UnixInputTask *) override;

    virtual std::optional<std::string> launch() override;

    virtual std::optional<std::string> wait() override;

	virtual void close_all_next() override;

	virtual void close_all_previous() override;

private:
	/**
	 * Stores the command representation.
	 */
	std::vector<std::string> arguments;
};
