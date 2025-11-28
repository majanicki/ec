ifeq ($(OS),Windows_NT)
    BUILD=build lab07
else
    BUILD=./build.sh lab07
endif
all:
	${BUILD}
