SOURCES := cash.cpp
EXECUTABLE := _bin/cash
SYNTAX_TEST := _tests/syn.cash

ifeq ($(OS),Windows_NT)
	EXECUTABLE := $(EXECUTABLE)_windows.exe
else
	EXECUTABLE := $(EXECUTABLE)_unix
endif

all: run

build:
	g++ $(SOURCES) -std=c++17 -o $(EXECUTABLE)

run: build
	$(EXECUTABLE)

test: build
	$(EXECUTABLE) < $(SYNTAX_TEST)