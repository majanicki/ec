ifeq ($(OS),Windows_NT)
    BUILD=build lab10
else
    BUILD=./build.sh lab10
endif
all:
	${BUILD}
