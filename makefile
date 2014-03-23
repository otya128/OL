PROGRAM := lang
SOURCES := $(wildcard *.cpp)
OBJS := $(SOURCES:.cpp=.o)
DEPENDS := $(SOURCES:.cpp=.depend)
CXX := g++
CPPFLAGS := -std=c++11 -O -Wall -W `pkg-config gtk+-2.0 --libs --cflags`
.PHONY: all
all: $(PROGRAM)
$(PROGRAM): $(OBJS) 
	$(CXX) -o $(PROGRAM) $(CPPFLAGS) $^


%.depend: %.cpp
	@echo generating $@
	@$(SHELL) -ec '$(CC) -MM $(CPPFLAGS) $< | sed "s/\($*\)\.o[ :]*/\1.o $@ : /g" > $@; [ -s $@ ] || rm -f $@'


ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPENDS)
endif

.PHONY : clean
clean:
	rm -f $(PROGRAM)
	rm -fr $(OBJS)
	rm -fr $(DEPENDS)
