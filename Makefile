NAME := minishell

CC := cc

LIBFT_DIR := ./libft
LIBFT := $(LIBFT_DIR)/libft.a
LIBFTFLAGS := -L$(LIBFT_DIR) -lft

CFLAGS = -Wall -Wextra -Werror -I. -I$(LIBFT_DIR) \
	-I./environment -I./pipeline -I./executor -I./sh -I./parser -I./errors -I./utils -I./expansion -I./heredoc

SH_SRC := \
	sh/sh_init.c \
	sh/sh_parse.c

UTILS_SRC := \
	utils/u_getcwd.c

ENV_SRC := \
	environment/env_lifecycle.c \
	environment/env_init.c \
	environment/env_parse.c \
	environment/env_utils.c \
	environment/env_operations.c \
	environment/env_normalize.c \
	environment/env_envp.c \
	environment/env_pairs.c \
	environment/env_key.c

HEREDOC_SRC := \
	heredoc/heredoc.c \
	heredoc/heredoc_write.c

PARSE_SRC := \
	parser/lexer/lexer.c \
	parser/lexer/lexer_lifecycle.c \
	parser/lexer/lexer_utils.c \
	parser/lexer/lexer_token_utils.c \
	parser/lexer/lexer_token.c \
	parser/lexer/lexer_word_stop_resolver.c \
	parser/lexer/lexer_count_word_pieces.c \
	parser/lexer/lexer_parse_utils.c \
	parser/lexer/lexer_parse_word.c \
	parser/lexer/lexer_parse_operator.c \
	parser/parser.c 

EXPANSION_SRC := \
	expansion/expansion.c

EXECUTOR_SRC := \
	executor/builtins/env.c \
	executor/builtins/unset.c \
	executor/builtins/export.c \
	executor/builtins/pwd.c

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
	$(SH_SRC) \
	$(UTILS_SRC) \
	$(ENV_SRC) \
	$(ERRORS_SRC) \
	$(EXECUTOR_SRC) \
	$(PARSE_SRC)

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