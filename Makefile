# This Makefile is based on Job Vranish's article at https://spin.atomicobject.com/2016/08/26/makefile-c-projects/

TARGET_EXEC ?= access-log-supervisor

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

SRCS := \
	src/utils/Address.cpp \
	src/main.cpp




# Automatic substitution
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CPPFLAGS ?= -MMD -MP

# Final executable
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


-include $(DEPS)

MKDIR_P ?= mkdir -p


# Utilities
.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)

.PHONY: doc
doc:
	mkdir -p build/doc
	doxygen doc/Doxyfile
