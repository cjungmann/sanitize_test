CFLAGS = -Wall -Werror -pedantic -fsanitize=address -fno-omit-frame-pointer -O0
CFLAGS += -std=c99 -m64 -ggdb

.PHONY: all
all: test test2 minimal

% : %.c
	gcc ${CFLAGS} -o $@ $<

