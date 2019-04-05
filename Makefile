SOURCES := cash.cpp
EXECUTABLE := bin/cash
SYNTAX_TEST := tests/syn.cash

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