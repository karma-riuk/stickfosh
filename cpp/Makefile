# Compiler flags (add -O2 for optimization, -g for debugging, etc.)
CXXFLAGS = -Wall -O3

# Name of the final executable
TARGET = stickfosh

# List of source files
SRCS = main.cpp board.cpp

# Automatically generate object file names from source file names
OBJS = $(SRCS:.cpp=.o)

# Default rule: build the target executable
all: $(TARGET)

# Link the object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Pattern rule: compile each .cpp file to a .o file
%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: board.o
stickfosh: main.o board.o

# Clean rule to remove built files
clean:
	rm -f $(TARGET) $(OBJS)

# Optional: a rule to rebuild everything from scratch
rebuild: clean all
