PROJECT:=argreffunc

SRC_DIR=src
OBJ_DIR=$(BUILD_DIR)/obj
BIN_DIR=$(BUILD_DIR)/bin

include MakefileFunctions.in

ifeq ($(UNAME_S), Darwin)
	CLANG_CXX_FLAGS+= -stdlib=libstdc++
endif

CXX=clang++
LD=clang++
CXXFLAGS= -std=c++1y -I./src/ -c $< -o $@
LDFLAGS=
LDSHARED=clang++ -Wall -shared -o $@ $^ $< $(LDFLAGS)
LDEXE=clang++ -Wall -o $@

ifeq ($(MAKECMDGOALS),debug)
	BUILD_DIR=debug
	CXXFLAGS+= -g
	LDFLAGS+= -g
else
	CXXFLAGS+= -O2
	BUILD_DIR=release
endif

HEADER_LIST:=$(PROJECT)/$(PROJECT).hpp
TEST_MACROS_HEADER:=$(SRC_DIR)/catch.hpp
AUTO_FILES:=$(HEADER_LIST) $(TEST_MACROS_HEADER)

SRC = $(filter-out $(AUTO_FILES),$(call rwildcard, $(SRC_DIR), *.cpp))
HEADERS = $(filter-out $(AUTO_FILES), $(call rwildcard, $(SRC_DIR), *.hpp))
OBJ = $(patsubst %.cpp,$(OBJ_DIR)/%.obj,$(SRC))

CPP_TEST_EXEC := $(sort $(patsubst %.cpp, $(BIN_DIR)/%.exe, $(call rwildcard, tests, *Main.cpp)))
CPP_TEST_OBJ := $(patsubst %.cpp,$(OBJ_DIR)/%.obj, $(call rwildcard, tests, *.cpp))

# force TEST_MACROS_HEADER to be retrieved here...
all: $(TEST_MACROS_HEADER) test

# this must be placed after your .DEFAULT_GOAL, or you can manually state what it is
# https://www.gnu.org/software/make/manual/html_node/Special-Variables.html
$(call create_dirs,$(OBJ) $(CPP_TEST_EXEC) $(CPP_TEST_OBJ))

debug: $(TEST_MACROS_HEADER) test

clean:
	-$(RM) -r ./debug/ ./release/

clobber:
	-$(RM) $(AUTO_FILES)
	-git clean -fxd --exclude .vscode/

$(HEADER_LIST): $(HEADERS)
	@echo "// this file is auto-generated, do not modify" > $@
	@$(foreach hpp,$^,echo '#include "$(hpp)"' >> $@;)
	@echo "" >> $@

test: test_cpp

test_cpp: $(CPP_TEST_EXEC) | $(TEST_MACROS_HEADER)
	$(foreach cpp_test_exec, $^, $(call colorecho,$(cpp_test_exec)))

$(CPP_TEST_EXEC): $(CPP_TEST_OBJ) | $(TEST_MACROS_HEADER)
	$(call colorecho,$(LDEXE) $(filter-out $(PROJECT)/$(PROJECT).hpp,$^) $(CLANG_LD_FLAGS))

$(OBJ_DIR)/%.obj: %.cpp %.hpp | $(TARGET_DIRS)
	$(call colorecho,$(CXX) $(CXXFLAGS))

$(OBJ_DIR)/%.obj: %.cpp | $(TARGET_DIRS)
	$(call colorecho,$(CXX) $(CXXFLAGS))

documentation: apidoc

apidoc: $(HEADERS) $(patsubst %.py,%.log,$(call rwildcard,examples,*.py))
	$(call colorecho,doxygen doxyconf)

$(TEST_MACROS_HEADER):
	$(call colorecho,wget https://raw.githubusercontent.com/philsquared/Catch/master/single_include/catch.hpp -O $@)

