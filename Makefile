CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRC = src/main.c src/parser.c src/executor.c src/builtins.c src/command.c src/tokenizer.c src/job.c
OBJ = $(SRC:.c=.o)

all: shell

shell: $(OBJ)
	$(CC) $(CFLAGS) -o shell $(OBJ)

clean:
	rm -f shell src/*.o