// Copyright (C) 2020 luna_koly
//
// This file defines common API for running
// processes in a pipeline.


#pragma once


#include <optional>
#include <string>
#include <sstream>


/**
 * Abstract pipeline entry with minimum
 * functionality.
 */
struct Task {
    virtual ~Task() {}

    /**
     * Launches the pipeline starting with
     * this task.
     */
    virtual std::optional<std::string> launch() = 0;
    /**
     * Waits until this and all subsequent tasks
     * finish.
     */
    virtual std::optional<std::string> wait() = 0;

    /**
     * DEALLOCATES all subsequent tasks.
     */
    virtual void terminate_all() = 0;
};


/**
 * Some task that we may pass further calculations
 * to.
 */
template <typename InputTask>
struct OutputTask : public Task {
    /**
     * Configures the previous task.
     * This function is typically called by
     * then() of the previous task.
     */
    virtual std::optional<std::string> configure(InputTask *) = 0;

    virtual void terminate_all() override {
        // chill
    }
};


/**
 * Some task that can then pass it's calculations
 * further.
 */
template <typename OutputTask>
struct InputTask : public Task {
    /**
	 * Pointer to the next task in the pipeline.
	 */
	OutputTask * next = nullptr;

    /**
     * Configures the next task.
     * Typically calls configure() of the
     * next task over itself.
     */
    virtual std::optional<std::string> then(OutputTask * next) = 0;

    virtual void terminate_all() override {
        if (next != nullptr) {
            delete next;
        }
    }
};


/**
 * A task that is an InputTask and an OutputTask
 * at the same time.
 */
template <typename InputTask, typename OutputTask>
struct DuplexTask : public InputTask, public OutputTask {};
