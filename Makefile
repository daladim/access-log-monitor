# This Makefile is based on Job Vranish's article at https://spin.atomicobject.com/2016/08/26/makefile-c-projects/

MAIN_EXEC ?= access-log-supervisor
TEST_EXEC ?= test

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

MAIN_SRCS := \
	src/utils/AddressRange.cpp \
	src/utils/Address.cpp \
	src/main.cpp

TEST_SRCS := \
	src/utils/AddressRange.cpp \
	src/utils/Address.cpp \
	src/utils/Timestamp.cpp \
	src/utils/sqlite/SQLiteDB.cpp \
	src/utils/sqlite/SQLiteStatement.cpp \
	src/utils/Database.cpp \
	tests/TestAddresses.cpp \
	tests/TestAliasStore.cpp \
	tests/TestSQLiteObjects.cpp \
	tests/TestTimestamp.cpp \
	tests/TestDatabase.cpp \
	tests/tests.cpp



MAIN_LDFLAGS = -lcidr -lsqlite3
TEST_LDFLAGS = -lcidr -lsqlite3



# Usual targets
all: $(BUILD_DIR)/$(MAIN_EXEC)
test: $(BUILD_DIR)/$(TEST_EXEC)

# Automatic substitution
MAIN_OBJS := $(MAIN_SRCS:%=$(BUILD_DIR)/%.o)
TEST_OBJS := $(TEST_SRCS:%=$(BUILD_DIR)/%.o)

ALL_DEPS := \
	$(MAIN_OBJS:.o=.d) \
	$(TEST_OBJS:.o=.d)

# For automatic dependancy detection
CPPFLAGS ?= -MMD -MP
# Other flags
CPPFLAGS += -std=c++17

# Final executable
$(BUILD_DIR)/$(MAIN_EXEC): $(MAIN_OBJS)
	$(CXX) $(MAIN_OBJS) -o $@ $(MAIN_LDFLAGS)
$(BUILD_DIR)/$(TEST_EXEC): $(TEST_OBJS)
	$(CXX) $(TEST_OBJS) -o $@ $(TEST_LDFLAGS)

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


-include $(ALL_DEPS)

MKDIR_P ?= mkdir -p


# Utilities
.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)

.PHONY: doc
doc:
	mkdir -p build/doc
	doxygen doc/Doxyfile
