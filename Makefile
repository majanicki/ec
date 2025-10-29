ifeq ($(OS),Windows_NT)
    BUILD=build lab04
else
    BUILD=./build.sh lab04
endif
all:
	${BUILD}
