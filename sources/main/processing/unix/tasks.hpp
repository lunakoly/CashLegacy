// Copyright (C) 2020 luna_koly
//
// Unix-specific tasks declarations.


#pragma once


#include "../tasks.hpp"

#include <cstring>


/**
 * An entity tham may close all it's
 * previous/next file descriptors.
 */
struct DescriptorsTerminator {
	virtual ~DescriptorsTerminator() {}

	/**
	 * Closes all own and next file descriptors.
	 */
	virtual void close_all_next() = 0;
	/**
	 * Closes all own and previous file descriptors.
	 */
	virtual void close_all_previous() = 0;
};


struct UnixInputTask;


/**
 * OutputTask with Unix-specific data.
 */
struct UnixOutputTask : public OutputTask<UnixInputTask>, public DescriptorsTerminator {
    /**
	 * File descriptor that acts as stdin for this task.
	 * For the sake of simplicity I leave a member here,
     * but it's only used within a Process instance, so we
     * may want to replace it with accessor methods,
     * but I'm too lazy to do it now.
	 */
	int input = 0;
	/**
	 * Pointer to the previous task in the pipeline.
	 * This is required to iterate all the
	 * tasks and close their file descriptors
	 * after fork()'ing each child.
	 */
	UnixInputTask * previous = nullptr;
};


/**
 * InputTask with Unix-specific data.
 */
struct UnixInputTask : public InputTask<UnixOutputTask>, public DescriptorsTerminator {
	/**
	 * File descriptor that acts as stdout for this task.
	 * For the sake of simplicity I leave a member here,
     * but it's only used within a Process instance, so we
     * may want to replace it with accessor methods,
     * but I'm too lazy to do it now.
	 */
	int output = 0;
};
