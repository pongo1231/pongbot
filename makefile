CXX ?= gcc

# path #
SRC_PATH = Pongbot
BUILD_PATH = build
BIN_PATH = $(BUILD_PATH)/bin

# executable # 
BIN_NAME = Pongbot.so

# extensions #
SRC_EXT = cpp

# code lists #
# Find all source files in the source directory, sorted by
# most recently modified
SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)
# Set the object file names, with the source directory stripped
# from the path, and the build path prepended in its place
OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
# Set the dependency files that will be used to add header dependencies
DEPS = $(OBJECTS:.o=.d)

# flags #
COMPILE_FLAGS = -Wall -Wextra -fpermissive -w -DPOSIX -Dstricmp=strcasecmp -D_stricmp=strcasecmp -D_strnicmp=strncasecmp \
	-Dstrnicmp=strncasecmp -D_snprintf=snprintf -D_vsnprintf=vsnprintf -D_alloca=alloca \
	-Dstrcmpi=strcasecmp -DCOMPILER_GCC -Wall -Wno-non-virtual-dtor -Wno-overloaded-virtual \
	-Werror -fPIC -fno-exceptions -fno-rtti -msse -m32 -fno-strict-aliasing -D_LINUX -shared -ggdb
	#-D_DEBUG
INCLUDES = -I $(MLIBS) -I $(MLIBS)/hlsdk $(addprefix -I,$(shell find $(MLIBS)/hlsdk/public -type d -print)) -I $(MLIBS)/hlsdk/engine \
	-I $(MLIBS)/hlsdk/mathlib -I $(MLIBS)/hlsdk/vstdlib -I $(MLIBS)/hlsdk/game -I $(MLIBS)/hlsdk/game/tier1 -I $(MLIBS)/hlsdk/game/tier0 \
	-I $(MLIBS)/hlsdk/tier0 -I $(MLIBS)/hlsdk/tier1 -I $(MLIBS)/hlsdk/game/server -I $(MLIBS)/metamod -I $(MLIBS)/metamod/sourcehook -I Pongbot

# Space-separated pkg-config libraries used by this project
LIBS = -L$(MLIBS)/hlsdk/lib/linux -ltier0_srv -lvstdlib_srv -l:mathlib_i486.a -l:tier1_i486.a -l:tier2_i486.a -l:tier3_i486.a -ldl

.PHONY: default_target
default_target: release

.PHONY: release
release: export CXXFLAGS := $(CXXFLAGS) $(COMPILE_FLAGS)
release: dirs
	@$(MAKE) all

.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(dir $(OBJECTS))
	@mkdir -p $(BIN_PATH)

.PHONY: clean
clean:
	@echo "Deleting $(BIN_NAME) symlink"
	@$(RM) $(BIN_NAME)
	@echo "Deleting directories"
	@$(RM) -r $(BUILD_PATH)
	@$(RM) -r $(BIN_PATH)

# Creation of the executable
.PHONY: all
all: $(BIN_PATH)/$(BIN_NAME)
$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	@echo "Linking: $@"
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LIBS) -o $@

# Add dependency files, if they exist
-include $(DEPS)

# Source file rules
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: $< -> $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -MP -MMD -c $< -o $@
