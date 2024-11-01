CC = cc
CFLAGS = -pedantic -Wall -Wextra -Iinclude
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c,obj/%.o,$(SRC))
LIB_DIR = lib
LIB = $(LIB_DIR)/libmystb.a

all: $(LIB)

$(LIB): $(OBJ) | $(LIB_DIR)
	@echo "Creating static library $(LIB)"
	ar rcs $@ $^

obj/%.o: src/%.c | obj
	@mkdir -p obj  # Ensure obj directory exists
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

test: $(LIB)
	$(CC) $(CFLAGS) -L$(LIB_DIR) tests/main.c -o tests/test -lmystb
	tests/./test

clean:
	rm -f $(OBJ) $(LIB) tests/test

.PHONY: all clean test
