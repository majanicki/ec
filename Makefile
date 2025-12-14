ifeq ($(OS),Windows_NT)
    BUILD=build lab09
else
    BUILD=./build.sh lab09
endif
all:
	${BUILD}
