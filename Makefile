.PHONY: all clean libs

CC := gcc
AS := gcc
LD := gcc
AR := ar

# discover lib/*/include dirs and convert to -I flags
LIB_INC_DIRS := $(shell find lib -maxdepth 2 -type d -name include 2>/dev/null)
LIB_INCLUDES := $(patsubst %,-I%,$(LIB_INC_DIRS))

CFLAGS := -c -O2 -std=c11 $(LIB_INCLUDES)
ASFLAGS := -c
LDFLAGS :=

ARCH ?= amd64

SRC_SRCS := $(shell find src -name '*.c' 2>/dev/null)
SRC_OBJS := $(SRC_SRCS:.c=.o)

# find top-level src projects (directory names under src/)
TOPS := $(sort $(foreach f,$(SRC_SRCS),$(firstword $(subst /, ,$(patsubst src/%,%,$(dir $(f)))))))
USER_BINS := $(patsubst %,build/%,$(TOPS))

# find top-level lib projects (directory names under lib/)
LIB_TOPS := $(notdir $(wildcard lib/*))
LIB_SRCS := $(foreach l,$(LIB_TOPS),$(shell find lib/$(l) -name '*.c' 2>/dev/null))
LIB_OBJS := $(LIB_SRCS:.c=.o)
USER_LIBS := $(patsubst %,build/lib%.so,$(LIB_TOPS))

# per-src-project config loader
define LOAD_PROJECT_CONFIG
CFLAGS_$(1) 	:=
LDFLAGS_$(1) 	:=
LDLIBS_$(1) 	:=

-include src/$(1)/.config
endef

$(foreach t,$(TOPS),$(eval $(call LOAD_PROJECT_CONFIG,$t)))

# per-lib-project config loader
define LOAD_LIB_PROJECT_CONFIG
CFLAGS_$(1) 	:=
LDFLAGS_$(1) 	:=
LDLIBS_$(1) 	:=

-include lib/$(1)/.config
endef

$(foreach t,$(LIB_TOPS),$(eval $(call LOAD_LIB_PROJECT_CONFIG,$t)))

all: libs $(USER_BINS)

# compile src C files with per-project CFLAGS_<project>
src/%.o: src/%.c
	$(CC) $(CFLAGS) $(CFLAGS_$(firstword $(subst /, ,$(patsubst src/%,%,$(dir $<))))) -o $@ $<

# link user binaries
define LINK_USER
build/$(1): $(filter src/$(1)/%,$(SRC_OBJS))
	mkdir -p build
	$(LD) $(LDFLAGS) $(LDFLAGS_$(1)) \
		-L build/ -o $$@ \
		$(filter src/$(1)/%,$(SRC_OBJS)) $(LDLIBS_$(1))
endef

$(foreach t,$(TOPS),$(eval $(call LINK_USER,$t)))

# compile lib C files with per-project CFLAGS_<project>
lib/%.o: lib/%.c
	$(CC) $(CFLAGS) $(CFLAGS_$(firstword $(subst /, ,$(patsubst lib/%,%,$(dir $<))))) -o $@ $<

# libraries target collects all lib shared objects

# link each lib/<proj> into build/lib<proj>.so
define LINK_LIB
build/lib$(1).so: $(filter lib/$(1)/%,$(LIB_OBJS))
	mkdir -p build
	$(LD) -shared $(LDFLAGS) $(LDFLAGS_$(1)) -o $$@ \
		$(filter lib/$(1)/%,$(LIB_OBJS)) $(LDLIBS_$(1))

endef

$(foreach t,$(LIB_TOPS),$(eval $(call LINK_LIB,$t)))

libs: $(USER_LIBS)

clean:
	find . -name '*.[s]o' -delete