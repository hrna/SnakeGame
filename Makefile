CXX := g++
CXXFLAGS := -Wall -Werror -MMD

SOURCES := $(wildcard *.cpp)
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
DEPS := ${OBJECTS:.o=.d}
TARGET := SnakeGame

.PHONY: clean

${TARGET}: ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@ -lncurses

-include ${DEPS}

clean:
	rm -f ${DEPS} ${OBJECTS} ${TARGET}
