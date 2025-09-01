NAME := minishell

CC := cc

LIBFT_DIR := ./libft
LIBFT := $(LIBFT_DIR)/libft.a
LIBFTFLAGS := -L$(LIBFT_DIR) -lft

CFLAGS = -Wall -Wextra -Werror -I. -I$(LIBFT_DIR) -I./environment -I./pipeline -I./executor

ENV_SRC := \
	environment/env_lifecycle.c \
	environment/env_init.c \
	environment/env_parse.c \
	environment/env_utils.c \
	environment/env_operations.c \
	environment/env_normalize.c \
	environment/env_envp.c \
	environment/env_pairs.c 


EXECUTOR_SRC := \
	executor/builtins/env.c

ERRORS_SRC := \
	errors/errors_reporter.c \
	errors/errors_env_msgs.c \
	errors/errors_parser_msgs.c \
	errors/errors_expansion_msg.c \
	errors/err_exec_msg.c \
	errors/errors_heredoc_msg.c \
	errors/errors_factory.c 

SRC := \
	main.c \
	$(ENV_SRC) \
	$(ERRORS_SRC) \
	$(EXECUTOR_SRC)

OBJ := $(SRC:.c=.o)


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