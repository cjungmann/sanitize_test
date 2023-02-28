CFLAGS = -Wall -Werror -pedantic -fsanitize=address -fno-omit-frame-pointer
# CFLAGS += std=c99 -m64 -ggdb

.PHONY: all
all: test test2

% : %.c
	gcc ${CFLAGS} -o $@ $<

