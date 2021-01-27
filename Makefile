CXX := g++
CXXFLAGS := -Wall -Werror -MMD

SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
DEPS := ${OBJECTS:.o=.d}
TARGET := SnakeGame

.PHONY: clean


${TARGET}: ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@ -lncurses

-include ${DEPS}

install:
	@mkdir build
	@mkdir build/obj
	@mkdir build/bin
	@mv ${DEPS} ${OBJECTS} build/obj/
	@mv ${TARGET} build/bin/

clean:
	@rm -rf build/
