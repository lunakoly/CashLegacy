// Copyright (C) 2020 luna_koly
//
// Unix-specific task for declaring a
// file input task.


#pragma once


#include "../tasks.hpp"

#include "tasks.hpp"


/**
 * Unix-specific task for declaring a
 * file input task.
 */
class UnixInputFile : public UnixInputTask {
public:
    UnixInputFile(const std::string & filename)
        : filename(filename) {}

    virtual std::optional<std::string> then(UnixOutputTask *) override;

    virtual std::optional<std::string> launch() override;

    virtual std::optional<std::string> wait() override;

    virtual void close_all_next() override;

	virtual void close_all_previous() override;

private:
	/**
	 * The file to be read.
	 */
	std::string filename;
};
