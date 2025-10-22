ifeq ($(OS),Windows_NT)
    BUILD=build lab03
else
    BUILD=./build.sh lab03
endif
all:
	${BUILD}
