# =============================================
# Universal Makefile for C++ Libraries
# Supports: Linux (.so) and macOS (.dylib)
# =============================================

# -------------------------------
# 1. System Detection
# -------------------------------
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
  LIB_EXT = .so
  LDFLAGS_SHARED = -shared -Wl,-soname,
else ifeq ($(UNAME_S),Darwin)
  LIB_EXT = .dylib
  LDFLAGS_SHARED = -dynamiclib -install_name @rpath/
endif

# -------------------------------
# 2. Build Configuration
# -------------------------------
# Default build type (override with: make BUILD_TYPE=Debug)
BUILD_TYPE ?= Release

# Validate build type
SUPPORTED_BUILD_TYPES = Release Debug RelWithDebInfo MinSizeRel
ifneq ($(filter $(BUILD_TYPE),$(SUPPORTED_BUILD_TYPES)),)
  $(info Build type: $(BUILD_TYPE))
else
  $(error Unsupported BUILD_TYPE. Valid options: $(SUPPORTED_BUILD_TYPES))
endif

# -------------------------------
# 3. Project Settings
# -------------------------------
PROJECT_NAME = libmodul
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
LIB_DIR = lib

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# -------------------------------
# 4. Compiler & Linker Flags
# -------------------------------
CXX = g++
CXXFLAGS_BASE = -std=c++17 -Wall -Wextra -I$(INC_DIR) -fPIC

# Build type specific flags
ifeq ($(BUILD_TYPE),Debug)
  CXXFLAGS = $(CXXFLAGS_BASE) -g -O0 -DDEBUG
  LDFLAGS = -g
else ifeq ($(BUILD_TYPE),RelWithDebInfo)
  CXXFLAGS = $(CXXFLAGS_BASE) -g -O2
  LDFLAGS = -g
else ifeq ($(BUILD_TYPE),MinSizeRel)
  CXXFLAGS = $(CXXFLAGS_BASE) -Os
else
  CXXFLAGS = $(CXXFLAGS_BASE) -O3 -DNDEBUG
endif

# Platform-specific flags
ifeq ($(UNAME_S),Darwin)
  CXXFLAGS += -arch arm64 # Universal binary for macOS
endif

# -------------------------------
# 5. Target Definitions
# -------------------------------
STATIC_LIB = $(LIB_DIR)/lib$(PROJECT_NAME).a
DYNAMIC_LIB = $(LIB_DIR)/lib$(PROJECT_NAME)$(LIB_EXT)

# -------------------------------
# 6. Build Rules
# -------------------------------
all: $(STATIC_LIB) $(DYNAMIC_LIB)

# Static library
$(STATIC_LIB): $(OBJS)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $(OBJS)

# Dynamic library
$(DYNAMIC_LIB): $(OBJS)
	@mkdir -p $(LIB_DIR)
	$(CXX) $(LDFLAGS) $(LDFLAGS_SHARED)$(notdir $(DYNAMIC_LIB)) $(OBJS) -o $@

# Object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# -------------------------------
# 7. Utility Targets
# -------------------------------
clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR)

install: $(STATIC_LIB) $(DYNAMIC_LIB)
	@mkdir -p /usr/local/lib
	@mkdir -p /usr/local/include
	cp $(STATIC_LIB) $(DYNAMIC_LIB) /usr/local/lib
	cp -r $(INC_DIR)/* /usr/local/include

.PHONY: all clean install

# =============================================
# Usage Examples:
# =============================================
# make                # Default Release build
# make BUILD_TYPE=Debug
# make clean          # Clean build artifacts
# make install        # Install to system (requires sudo)

# Test configuration
TEST_DIR = tests
TEST_BUILD_DIR = $(BUILD_DIR)/tests
TEST_EXEC = $(BUILD_DIR)/$(PROJECT_NAME)_tests
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS = $(TEST_SRCS:$(TEST_DIR)/%.cpp=$(TEST_BUILD_DIR)/%.o)

# Google Test paths (update these to match your brew installation)
GTEST_INC = -I/opt/homebrew/Cellar/googletest/1.17.0/include
GTEST_LIB = -L/opt/homebrew/Cellar/googletest/1.17.0/lib -lgtest -lgtest_main -lpthread

# Test targets
.PHONY: test

test: $(TEST_EXEC)
	@echo "Running tests..."
	@./$(TEST_EXEC)

$(TEST_EXEC): $(OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(GTEST_LIB)

$(TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(GTEST_INC) -c $< -o $@