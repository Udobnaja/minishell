NAME := minishell

CC := cc

CFLAGS := -Wall -Wextra -Werror -I.  

SRC := \
	main.c \
	environment/env_lifecycle.c \
	environment/env_init.c \
	environment/env_parse.c \
	environment/env_utils.c \
	environment/env_operations.c \
	environment/env_normalize.c

OBJ := $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -lreadline

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re