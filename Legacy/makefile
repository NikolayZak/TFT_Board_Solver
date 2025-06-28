# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -Ofast -I../Crow/include

# Linker flags (external libraries)
LDFLAGS := -lsqlite3

# Target executable name
TARGET := api

# Source files
SRCS := FastVector.cpp Common.cpp TopScoringBoards.cpp Serialisation.cpp LocalDB.cpp Board.cpp Solver.cpp JobManager.cpp RequestHandler.cpp api.cpp
OBJS := $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files to create binary
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile .cpp files to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run