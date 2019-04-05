SOURCES=cash.cpp
EXECUTABLE=bin/cash
SYNTAX_TEST=tests/syn.cash

all: run

build:
	g++ $(SOURCES) -o $(EXECUTABLE)

run: build
	$(EXECUTABLE)

test: build
	$(EXECUTABLE) < $(SYNTAX_TEST)