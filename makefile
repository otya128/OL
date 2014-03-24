#http://d.hatena.ne.jp/td2sk/20110618/1308395998
PROGRAM := lang
SOURCES := $(wildcard *.cpp)
OBJS := $(SOURCES:.cpp=.o)
DEPEND_DIR := depend
DEPENDS := $(addprefix $(DEPEND_DIR)/,$(SOURCES:.cpp=.depend))
<<<<<<< HEAD
CXX := g++
CPPFLAGS := -std=c++11 `pkg-config gtk+-2.0 --libs --cflags` -fpermissive  -w $(DEBUG) -U_WIN32 
=======
CXX := clang++
CPPFLAGS := -std=c++11 `pkg-config gtk+-2.0 --libs --cflags` -fpermissive  -w $(DEBUG) 
>>>>>>> 4433482f70c4800b06ec420021eb3790717ccdfd
.PHONY: all
all: $(PROGRAM)
$(PROGRAM): $(OBJS) 
	$(CXX) -o $(PROGRAM) $^ $(CPPFLAGS)


$(DEPEND_DIR)/%.depend: %.cpp
	@echo generating $@
	@mkdir -p $(DEPEND_DIR)
	@$(SHELL) -ec '$(CXX) $(CXXFLAGS) $(LIBS) -MM $< | tr "\\\\\n" " " | sed -e "s/^\(.*\)$$/$(OBJ_DIR)\/\1\n\t@mkdir -p $$\(OBJ_DIR\)\n\t$$\(CXX\) $$\(CXXFLAGS\) -c $$< -o $$\@ $$\(LIBS\)/g" > $(DEPEND_DIR)/$(notdir $@)'


ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPENDS)
endif

.PHONY : clean
clean:
	rm -f $(PROGRAM)
	rm -fr $(OBJS)
	rm -fr $(DEPENDS)
