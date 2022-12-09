# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image gtk+-3.0` -D __NO_INLINE__
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image gtk+-3.0` -lm

TARGET_EXEC := final_program

BUILD_DIR := ./build
SRC_DIRS := ./src

# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. Make will incorrectly expand these otherwise.
SRCS := $(shell find $(SRC_DIRS) -name '*.c' -not -path "*/no-make/*")

# String substitution for every C/C++ file.
# As an example, hello.cpp turns into ./build/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ ${CFLAGS} $(LDFLAGS) ${LDLIBS}

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) ${LDFLAGS} ${LDLIBS} -c $< -o $@


.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)

# END
