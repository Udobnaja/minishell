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
	environment/env_normalize.c \
	environment/env_envp.c \
	environment/env_pairs.c

OBJ := $(SRC:.c=.o)

LIBFT_DIR := ./libft
LIBFT := $(LIBFT_DIR)/libft.a
LIBFTFLAGS := -L$(LIBFT_DIR) -lft

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean


fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re