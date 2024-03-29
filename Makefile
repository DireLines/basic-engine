bogus_name := $(shell python3 codegen.py)

TARGET_EXEC ?= Game

BUILD_DIR ?= ./bin
SRC_DIRS ?= ./src ./resources/scripts

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

#compile & link SDL
SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(SDL_CFLAGS) $(INC_FLAGS) -MMD -MP -std=c++11 -g -O2 -F/Library/Frameworks -DLOGS=$(LOGS)

#LINKER_FLAGS specifies the libraries we're linking against
LDFLAGS = $(SDL_LDFLAGS) -lSDL2_image -lSDL2_ttf

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR) ./src/generated/

-include $(DEPS)

MKDIR_P ?= mkdir -p
