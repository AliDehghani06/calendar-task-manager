CXX = g++
CXXFLAGS = -std=c++20 -Wall -pedantic

BUILD_DIR = build
SRC_DIR = src
OUT_EXE = UTrello

ifeq ($(OS),Windows_NT)
    LDLIBS += -l Ws2_32
endif

LIB_CPP := \
	utils/template_parser.cpp \
	utils/response.cpp \
	utils/request.cpp \
	utils/utilities.cpp \
	utils/strutils.cpp \
	server/server.cpp \
	server/route.cpp

LIB_OBJ := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(LIB_CPP))
SRC_CPP := $(wildcard $(SRC_DIR)/*.cpp)
SRC_OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/$(SRC_DIR)/%.o,$(SRC_CPP))
OBJS := $(LIB_OBJ) $(SRC_OBJ)

all: $(OUT_EXE)

$(OUT_EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/utils/%.o: utils/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/server/%.o: server/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean
clean:
ifeq ($(OS),Windows_NT)
	del /Q /S $(subst /,\,$(BUILD_DIR)) 2> nul & del /Q *.out 2> nul
else
	rm -rf $(BUILD_DIR) *.out
endif
