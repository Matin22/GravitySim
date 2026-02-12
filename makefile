# Makefile

# Compiler and flags
CXX := g++.exe
CXXFLAGS := -g -std=c++17 -Wall -Wextra -Iinclude 

# Source files
SRC := src/*.cpp src/glad.c

# Output
BIN_DIR := bin
TARGET := $(BIN_DIR)/main.exe

# Libraries
LIBS := -Llib/GLFW -lopengl32 -lglfw3 -lgdi32

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(SRC)
# 	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)

# Clean target
clean:
	rm -rf $(BIN_DIR)

# Run the executable (optional)
run: $(TARGET)
	./$(TARGET)