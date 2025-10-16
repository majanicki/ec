ifeq ($(OS),Windows_NT)
    BUILD=build lab02
else
    BUILD=./build.sh lab02
endif
all:
	${BUILD}
