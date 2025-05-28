APP_NAME = maps
LIB_NAME = libmaps
TEST_NAME = test_maps
LANG_DIR = c

CFLAGS = -Wall -Werror -g
CPPFLAGS = -I c/src -I c/thirdparty -MP -MMD
VALFLAGS = --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 
XML_FLAGS = --xml=yes --xml-file=
XML_MAIN_PATH = c/memcheck/XMLmain_prog.log
XML_TEST_PATH = 

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = test

APP_PATH = $(LANG_DIR)/$(BIN_DIR)/$(APP_NAME)
LIB_PATH = $(LANG_DIR)/$(OBJ_DIR)/$(SRC_DIR)/$(LIB_NAME)/$(LIB_NAME).a
TEST_PATH = $(LANG_DIR)/$(BIN_DIR)/$(TEST_NAME)

SRC_EXT = c
OBJ_EXT = o

APP_SRCS = $(shell find $(LANG_DIR)/$(SRC_DIR)/$(APP_NAME) -name '*.$(SRC_EXT)')
APP_OBJS = $(APP_SRCS:$(LANG_DIR)/$(SRC_DIR)/%.$(SRC_EXT)=$(LANG_DIR)/$(OBJ_DIR)/$(SRC_DIR)/%.$(OBJ_EXT))

LIB_SRCS = $(shell find $(LANG_DIR)/$(SRC_DIR)/$(LIB_NAME) -name '*.$(SRC_EXT)')
LIB_OBJS = $(LIB_SRCS:$(LANG_DIR)/$(SRC_DIR)/%.$(SRC_EXT)=$(LANG_DIR)/$(OBJ_DIR)/$(SRC_DIR)/%.$(OBJ_EXT))

TEST_SRCS = $(shell find $(LANG_DIR)/$(TEST_DIR) -name '*.$(SRC_EXT)')
TEST_OBJS = $(TEST_SRCS:$(LANG_DIR)/$(TEST_DIR)/%.$(SRC_EXT)=$(LANG_DIR)/$(OBJ_DIR)/$(TEST_DIR)/%.$(OBJ_EXT))
TEST_DEPS = $(LANG_DIR)/$(SRC_DIR)/mapconfig/mapconfig.${SRC_EXT}

DEPS = $(APP_OBJS:.$(OBJ_EXT)=.d) $(LIB_OBJS:.$(OBJ_EXT)=.d) $(LIB_OBJS:.$(OBJ_EXT)=.d) $(TEST_OBJS:.$(OBJ_EXT)=.d)

.PHONY: all
all: $(APP_PATH)

-include $(DEPS)

$(APP_PATH): $(APP_OBJS) $(LIB_PATH) $(LIB_PATH)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

$(LIB_PATH): $(LIB_OBJS)
	ar rcs $@ $^


$(LANG_DIR)/$(OBJ_DIR)/%.$(OBJ_EXT): $(LANG_DIR)/%.$(SRC_EXT)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(TEST_PATH): $(TEST_OBJS) $(TEST_DEPS) $(LIB_PATH)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

.PHONY: run
run: $(APP_PATH)
	$(APP_PATH)	
	
.PHONY: memcheck
memcheck: $(APP_PATH)
# valgrind $(VALFLAGS) $(XML_FLAGS)$(XML_MAIN_PATH) $(APP_PATH) 
	valgrind-ci $(XML_MAIN_PATH) $(VALFLAGS)

.PHONY: test
test: $(TEST_PATH)
	$(TEST_PATH) 

.PHONY: memcheck_tests
memcheck_tests: $(TEST_PATH)
	valgrind $(VALFLAGS) $(TEST_PATH) 


.PHONY: clean
clean:
	$(RM) $(APP_PATH) $(LIB_PATH) $(TEST_PATH)
	find $(LANG_DIR)/$(OBJ_DIR) -name '*.$(OBJ_EXT)' -exec $(RM) '{}' \;
	find $(LANG_DIR)/$(OBJ_DIR) -name '*.d' -exec $(RM) '{}' \;

.PHONY: init
init:
	mkdir c/bin
	mkdir c/obj
	mkdir c/obj/src
	mkdir c/obj/src/libmaps
	mkdir c/obj/src/maps
	mkdir c/obj/test
	mkdir c/memcheck