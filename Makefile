CC = cc
CFLAGS = -Wall -Wextra -I.
SRC = cursor.c graph.c list.c map.c path.c priorityQueue.c queue.c shared.c stack.c str.c
OBJ = $(patsubst %.c,obj/%.o,$(SRC))
LIB_DIR = lib
LIB = $(LIB_DIR)/libmystb.a

all: $(LIB)

$(LIB): $(OBJ) | $(LIB_DIR)
	@echo "Creating static library $(LIB)"
	ar rcs $@ $^

# Rule to compile each source file into the obj folder
obj/%.o: %.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure obj and lib directories exist
obj:
	mkdir -p obj

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

clean:
	@echo "Cleaning up..."
	rm -f $(OBJ) $(LIB)

.PHONY: all clean install uninstall
