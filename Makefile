NAME := minishell

CC := cc

CFLAGS := -Wall -Wextra -Werror -I.

ENV_SRC := \
	environment/env_lifecycle.c \
	environment/env_init.c \
	environment/env_parse.c \
	environment/env_utils.c \
	environment/env_operations.c \
	environment/env_normalize.c \
	environment/env_envp.c \
	environment/env_pairs.c

ERRORS_SRC := \
	errors/errors_reporter.c \
	errors/errors_env.c \
	errors/errors_parser.c \
	errors/errors_factory.c

SRC := \
	main.c \
	$(ENV_SRC) \
	$(ERRORS_SRC)

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