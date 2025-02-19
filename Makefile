CXXFLAGS += -O3 -Wall
# CXXFLAGS += -pg

# Add .d to Make's recognized suffixes.
SUFFIXES += .d

#We don't need to clean up when we're making these targets
NODEPS:=clean tags svn
#Find all the C++ files in the src/ directory
SOURCES:=$(shell find src/ -name "*.cpp")
OBJFILES := $(patsubst src/%.cpp,obj/%.o,$(SOURCES))
#These are the dependency files, which make will clean up after it creates them
DEPFILES:=$(patsubst %.cpp,%.d,$(SOURCES))

#Don't create dependencies when we're cleaning, for instance
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
	#Chances are, these files don't exist.  GMake will create them and
	#clean up automatically afterwards
	-include $(DEPFILES)
endif

#This is the rule for creating the dependency files
src/%.d: src/%.cpp
	$(CXX) $(CXXFLAGS) -MM -MT '$(patsubst src/%.cpp,obj/%.o,$<)' $< -MF $@

#This rule does the compilation
obj/%.o:
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

stickfosh: $(OBJFILES)
	$(CXX) $(CXXFLAGS) $(OBJFILES) $(LOADLIBES) $(LDLIBS) -o stickfosh -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -rf obj/* $(DEPFILES) test_bin/


# --- Test Support ---
# Find all test source files in the tests directory
TESTS := $(shell find tests -name "*.cpp")
# Define corresponding test executable names, e.g. tests/foo.cpp -> test_bin/foo
TEST_BIN := $(patsubst tests/%.cpp,test_bin/%,$(TESTS))
LIBS := $(filter-out obj/main.o,$(OBJFILES))

# Pattern rule: how to build a test executable from a test source file.
# You can adjust CXXFLAGS or add include directories if needed.
test_bin/%: tests/%.cpp $(LIBS)
	@echo $(LIBS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS) -lsfml-graphics -lsfml-window -lsfml-system

# The 'test' target builds all tests and then runs each one.
.PHONY: test
test: $(TEST_BIN)
	@echo "Running all tests..."
	@for t in $(TEST_BIN); do \
	    echo "---- Running $$t ----"; \
	    ./$$t || exit 1; \
	done
