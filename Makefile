APP_NAME = maps
LIBMAPS_NAME = libmaps
LIBLIST_NAME = liblist
TEST_NAME = test_maps
LANG_DIR = c

CFLAGS = -Wall -Werror -g
CPPFLAGS = -I c/src -I c/thirdparty -MP -MMD
VALFLAGS = --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = test

APP_PATH = $(LANG_DIR)/$(BIN_DIR)/$(APP_NAME)
LIBMAPS_PATH = $(LANG_DIR)/$(OBJ_DIR)/$(SRC_DIR)/$(LIBMAPS_NAME)/$(LIBMAPS_NAME).a
LIBLIST_PATH = $(LANG_DIR)/$(OBJ_DIR)/$(SRC_DIR)/$(LIBLIST_NAME)/$(LIBLIST_NAME).a
TEST_PATH = $(LANG_DIR)/$(BIN_DIR)/$(TEST_NAME)

SRC_EXT = c
OBJ_EXT = o

APP_SRCS = $(shell find $(LANG_DIR)/$(SRC_DIR)/$(APP_NAME) -name '*.$(SRC_EXT)')
APP_OBJS = $(APP_SRCS:$(LANG_DIR)/$(SRC_DIR)/%.$(SRC_EXT)=$(LANG_DIR)/$(OBJ_DIR)/$(SRC_DIR)/%.$(OBJ_EXT))

LIBMAPS_SRCS = $(shell find $(LANG_DIR)/$(SRC_DIR)/$(LIBMAPS_NAME) -name '*.$(SRC_EXT)')
LIBMAPS_OBJS = $(LIBMAPS_SRCS:$(LANG_DIR)/$(SRC_DIR)/%.$(SRC_EXT)=$(LANG_DIR)/$(OBJ_DIR)/$(SRC_DIR)/%.$(OBJ_EXT))

LIBLIST_SRCS = $(shell find $(LANG_DIR)/$(SRC_DIR)/$(LIBLIST_NAME) -name '*.$(SRC_EXT)')
LIBLIST_OBJS = $(LIBLIST_SRCS:$(LANG_DIR)/$(SRC_DIR)/%.$(SRC_EXT)=$(LANG_DIR)/$(OBJ_DIR)/$(SRC_DIR)/%.$(OBJ_EXT))

TEST_SRCS = $(shell find $(LANG_DIR)/$(TEST_DIR) -name '*.$(SRC_EXT)')
TEST_OBJS = $(TEST_SRCS:$(LANG_DIR)/$(TEST_DIR)/%.$(SRC_EXT)=$(LANG_DIR)/$(OBJ_DIR)/$(TEST_DIR)/%.$(OBJ_EXT))
TEST_DEPS = $(LANG_DIR)/$(SRC_DIR)/mapconfig/mapconfig.${SRC_EXT}

DEPS = $(APP_OBJS:.$(OBJ_EXT)=.d) $(LIBLIST_OBJS:.$(OBJ_EXT)=.d) $(LIBMAPS_OBJS:.$(OBJ_EXT)=.d) $(TEST_OBJS:.$(OBJ_EXT)=.d)

.PHONY: all
all: $(APP_PATH)

-include $(DEPS)

$(APP_PATH): $(APP_OBJS) $(LIBMAPS_PATH) $(LIBLIST_PATH)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

$(LIBMAPS_PATH): $(LIBMAPS_OBJS)
	ar rcs $@ $^

$(LIBLIST_PATH): $(LIBLIST_OBJS)
	ar rcs $@ $^


$(LANG_DIR)/$(OBJ_DIR)/%.$(OBJ_EXT): $(LANG_DIR)/%.$(SRC_EXT)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(TEST_PATH): $(TEST_OBJS) $(TEST_DEPS) $(LIBMAPS_PATH) $(LIBLIST_PATH)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

.PHONY: run
run: $(APP_PATH)
	$(APP_PATH)	
	
.PHONY: memcheck
memcheck: $(APP_PATH)
	valgrind $(VALFLAGS) $(APP_PATH) 

.PHONY: test
test: $(TEST_PATH)
	$(TEST_PATH) 
# .PHONY: memcheck_tests
# memcheck_tests: $(TEST_PATH)
# 	valgrind $(VALFLAGS) $(TEST_PATH) < $(INPUT_FILES_PATH)


.PHONY: clean
clean:
	$(RM) $(APP_PATH) $(LIBMAPS_PATH) 
	find $(LANG_DIR)/$(OBJ_DIR) -name '*.$(OBJ_EXT)' -exec $(RM) '{}' \;
	find $(LANG_DIR)/$(OBJ_DIR) -name '*.d' -exec $(RM) '{}' \;

.PHONY: init
init:
	mkdir c/bin
	mkdir c/obj
	mkdir c/obj/src
	mkdir c/obj/src/libmaps
	mkdir c/obj/src/liblist
	mkdir c/obj/src/maps
	mkdir c/obj/test
