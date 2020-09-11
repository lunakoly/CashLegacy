// Copyright (C) 2020 luna_koly
//
// This is just a helper abstraction
// that keeps track of internal errors itself.


#pragma once


#include <optional>
#include <string>

#include "tasks.hpp"

#include <vector>


#ifdef _WIN32
    #include "windows/tasks.hpp"
    #include "windows/input_file.hpp"
    #include "windows/output_file.hpp"
    #include "windows/process.hpp"
#else
	#include "unix/tasks.hpp"
    #include "unix/input_file.hpp"
    #include "unix/output_file.hpp"
    #include "unix/process.hpp"
#endif


#ifdef _WIN32
	#define InputTask  WindowsInputTask
    #define OutputTask WindowsOutputTask
    #define InputFile  WindowsInputFile
    #define OutputFile WindowsOutputFile
    #define Process    WindowsProcess
#else
	#define InputTask  UnixInputTask
    #define OutputTask UnixOutputTask
    #define InputFile  UnixInputFile
    #define OutputFile UnixOutputFile
    #define Process    UnixProcess
#endif


/**
 * A facade for launching the pipeline
 * and managing errors. It accumulates errors
 * inside itself and thus adds more flexibility
 * to the end-user API.
 *
 * A Launcher is called 'invalid' if it's
 * error optional is not empty.
 */
template <typename IT = InputTask, typename OT = OutputTask>
class TaskLauncher {
public:
	TaskLauncher(IT * initial) : initial(initial), previous(initial) {}

	/**
	 * Links the previous task
	 * to the next one.
	 * Does nothing if invalid.
	 */
	TaskLauncher<IT, OT> & then(OT * next) {
		if (previous == nullptr) {
			error = "Launcher > Attempt to chain another Task to an OutputTask.";
		} else if (!error) {
			error = previous->then(next);
			previous = nullptr;
		}

		return *this;
	}
	/**
	 * Links the previous task
	 * to the next one.
	 * Does nothing if invalid.
	 */
	TaskLauncher<IT, OT> & then(DuplexTask<IT, OT> * next) {
		// only succeeds if `then()` has been
		// called for an `OT *`
		if (previous == nullptr) {
			error = "Launcher > Attempt to chain another Task to an OutputTask.";
		} else if (!error) {
			error = previous->then(next);
			previous = next;
		}

		return *this;
	}

	/**
	 * Launches all the tasks starting with
	 * the initial one.
	 * Does nothing if invalid.
	 */
	TaskLauncher<IT, OT> & launch() {
		if (!error) {
			error = initial->launch();
		}

		return *this;
	}

	/**
	 * Asks each task starting with
	 * the initial one to wait until
	 * it's job is finished.
	 * Does nothing if invalid.
	 * Returns the error if it occurres.
	 */
	std::optional<std::string> wait() {
		if (!error) {
			error = initial->wait();
		}

		terminate_all();
		return error;
	}

	/**
	 * Returns an error value.
	 */
	std::optional<std::string> get_error() const {
		std::cout << "!!{%} " << error.has_value() << " [" << error.value() << "]" << std::endl;
		return error;
	}

	/**
	 * Frees all tasks passed as parameters
	 * to then(). It's done automatically after
	 * wait().
	 */
	TaskLauncher<IT, OT> & terminate_all() {
		initial->terminate_all();
		delete initial;
		return *this;
	}

private:
	/**
	 * Stored to support launch() and wait().
	 */
	IT * initial;
	/**
	 * Stored to support then().
	 */
	IT * previous;
	/**
	 * Determines the current state (invalidness).
	 */
	std::optional<std::string> error;
};


namespace Processing {
	/**
	 * Constructs a new Launcher with
	 * task being set as the initial one.
	 */
	static TaskLauncher<InputTask, OutputTask> start(InputTask * task) {
		return TaskLauncher<InputTask, OutputTask>{task};
	}
}


/**
 * Just a prettier alias.
 */
using Launcher = TaskLauncher<>;
