// Copyright (C) 2020 luna_koly
//
// Unix-specific task for declaring a
// file output task.


#pragma once


#include "../tasks.hpp"

#include "tasks.hpp"


/**
 * Unix-specific task for declaring a
 * file output task.
 */
class UnixOutputFile : public UnixOutputTask {
public:
    UnixOutputFile(const std::string & filename)
        : filename(filename) {}

    virtual std::optional<std::string> configure(UnixInputTask *) override;

    virtual std::optional<std::string> launch() override;

    virtual std::optional<std::string> wait() override;

    virtual void close_all_next() override;

	virtual void close_all_previous() override;

private:
	/**
	 * The file to write.
	 */
	std::string filename;
};

