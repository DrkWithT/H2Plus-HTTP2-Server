# Makefile
# Project: H2Plus Server (h2c server)
# Derek Tan

# compiler vars
CXX := g++ -std=c++17
CXXFLAGS := -Wall -Wextra -Werror -g

# executable dir
BIN_DIR := ./bin

# important dirs
BUILD_DIR := ./build
MAIN_DIR := ./mains
SRC_DIR := ./src

# auto generate source targets
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
MAINS := $(shell find $(MAIN_DIR) -name '*.cpp')
ALL_SRCS := $(SRCS) $(MAINS)

# auto generate object targets
SRCS_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
MAINS_OBJS := $(patsubst $(MAIN_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(MAINS))

# auto generate executable targets
EXECS := $(patsubst $(BUILD_DIR)/%.o,$(BIN_DIR)/%,$(MAINS_OBJS))

# search ./src and ./test for C sources
vpath %.cpp $(MAIN_DIR)
vpath %.{cpp,hpp} $(SRC_DIR)

.PHONY: sloc tell all objs execs clean

# utility rule: show SLOC
sloc:
	@wc -l $(MAIN_DIR)/*.cpp $(SRC_DIR)/**/*.hpp $(SRC_DIR)/*.cpp

# debug rule: show all targets and deps
tell:
	@echo "Module Sources:"
	@echo $(SRCS)
	@echo "Driver Sources:"
	@echo $(MAINS)
	@echo "All Objs"
	@echo $(SRCS_OBJS)
	@echo $(MAINS_OBJS)
	@echo "All Executables:"
	@echo $(EXECS)

# entry build rule
all: objs execs

# object file build stage
objs: $(SRCS_OBJS) $(MAINS_OBJS)

# executable link stage
execs: $(EXECS)

# sub-rules
$(BIN_DIR)/%: $(BUILD_DIR)/%.o $(SRCS_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -I$(SRC_DIR) -o $@

$(BUILD_DIR)/%.o: $(MAIN_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -I$(SRC_DIR) -o $@

# clean rule: only remove old executables!
clean:
	rm -f $(EXECS) $(BUILD_DIR)/*.o
