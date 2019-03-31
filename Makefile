SOURCES=cash.cpp
EXECUTABLE=cash.exe

all: run

build:
	g++ $(SOURCES) -o $(EXECUTABLE)

run: build
	cash

test: build
	cash < test.cash

single:
	g++ singlefile.cpp -o $(EXECUTABLE)
	cash < test.cash