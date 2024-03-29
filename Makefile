OBJS_DIR = .objs

# define all the executables
EXE_EDITOR=editor
EXE_EDITOR_TEST=editor_test
EXES_APPLICATION=$(EXE_EDITOR) $(EXE_EDITOR_TEST)

# list object file dependencies for each
OBJS_EDITOR=editor.o editor_main.o format.o
OBJS_EDITOR_TEST=editor.o editor_test.o format.o

# set up compiler
CC = clang
WARNINGS = -Wall -Wextra -Werror -Wno-error=unused-parameter
INCLUDES=-I./includes/
CFLAGS_DEBUG   = $(INCLUDES) -O0 $(WARNINGS) -g -std=c99 -c -MMD -MP -D_GNU_SOURCE
CFLAGS_RELEASE = $(INCLUDES) -O2 $(WARNINGS) -g -std=c99 -c -MMD -MP -D_GNU_SOURCE

# set up linker
LD = clang
LDFLAGS = -Llibs/ -lprovided

.PHONY: all
all: release

# build types
.PHONY: release
.PHONY: debug

release: $(EXES_APPLICATION)
debug:   clean $(EXES_APPLICATION:%=%-debug)

# include dependencies
-include $(OBJS_DIR)/*.d

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

# patterns to create objects
# keep the debug and release postfix for object files so that we can always
# separate them correctly
$(OBJS_DIR)/%-debug.o: %.c | $(OBJS_DIR)
	$(CC) $(CFLAGS_DEBUG) $< -o $@

$(OBJS_DIR)/%-release.o: %.c | $(OBJS_DIR)
	$(CC) $(CFLAGS_RELEASE) $< -o $@

# exes
# you will need a pair of exe and exe-debug targets for each exe
$(EXE_EDITOR)-debug: $(OBJS_EDITOR:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ $(LDFLAGS) -o $@

$(EXE_EDITOR): $(OBJS_EDITOR:%.o=$(OBJS_DIR)/%-release.o)
	$(LD) $^ $(LDFLAGS) -o $@

$(EXE_EDITOR_TEST)-debug: $(OBJS_EDITOR_TEST:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ $(LDFLAGS) -o $@

$(EXE_EDITOR_TEST): $(OBJS_EDITOR_TEST:%.o=$(OBJS_DIR)/%-release.o)
	$(LD) $^ $(LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -rf .objs $(EXES_APPLICATION) $(EXES_APPLICATION:%=%-debug)