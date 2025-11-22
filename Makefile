ifeq ($(OS),Windows_NT)
    BUILD=build lab06
else
    BUILD=./build.sh lab06
endif
all:
	${BUILD}
