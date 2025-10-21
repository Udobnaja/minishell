NAME := minishell

CC := cc

CFLAGS = -Wall -Wextra -Werror -I. -I$(LIBFT_DIR) \
	-I./environment -I./pipeline -I./executor -I./sh -I./parser -I./errors -I./utils -I./expansion -I./heredoc

UNAME_S := $(shell uname -s)

LDLIBS_READLINE ?=

ifeq ($(UNAME_S),Darwin)
READLINE_PREFIX := $(shell brew --prefix readline 2>/dev/null)
ifneq ($(READLINE_PREFIX),)
CFLAGS += -I$(READLINE_PREFIX)/include
LDLIBS_READLINE += -L$(READLINE_PREFIX)/lib -lreadline
else
LDLIBS_READLINE += -lreadline
endif
else
CFLAGS += -D_DEFAULT_SOURCE
LDLIBS_READLINE += -lreadline
endif

LIBFT_DIR := ./libft
LIBFT := $(LIBFT_DIR)/libft.a
LIBFTFLAGS := -L$(LIBFT_DIR) -lft



SH_SRC := \
	sh/sh_tempdir.c \
	sh/sh_status.c \
	sh/sh_termios.c \
	sh/sh_termios_utils.c \
	sh/sh_signals.c \
	sh/sh_rl_hook.c

UTILS_SRC := \
	utils/u_getcwd.c \
	utils/u_file.c

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
	heredoc/heredoc_write.c \
	heredoc/heredoc_write_line.c \
	heredoc/heredoc_store.c

PIPELINE_SRC := \
	pipeline/pipeline_destroy.c \
	pipeline/pipeline_init.c \
	pipeline/pipeline_operations.c

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
	parser/parser.c \
	parser/parser_utils.c \
	parser/parser_count_word.c \
	parser/parser_join_word.c \
	parser/parser_word_pipeline.c \
	parser/parser_ambiguous.c \
	parser/parser_ambiguous_utils.c \
	parser/parser_redirect_pipeline.c \
	parser/parser_append_pieces.c \
	parser/parser_cmd.c \
	parser/parser_append_expanded_key.c

EXPANSION_SRC := \
	expansion/expansion.c \
	expansion/expansion_key.c

EXECUTOR_SRC := \
	executor/builtins/env.c \
	executor/builtins/unset.c \
	executor/builtins/export.c \
	executor/builtins/pwd.c \
	executor/builtins/echo.c \
	executor/builtins/cd.c \
	executor/builtins/exit.c \
	executor/executor.c \
	executor/external_cmd.c \
	executor/exec_pipe.c \
	executor/exec_redirect.c \
	executor/exec_child_process_clean.c

ERRORS_SRC := \
	errors/errors_reporter.c \
	errors/errors_env_msgs.c \
	errors/errors_parser_msgs.c \
	errors/err_exec_msg.c \
	errors/errors_heredoc_msg.c \
	errors/errors_factory.c \
	errors/errors_reporter_utils.c

SRC := \
	main.c \
	msh_init.c \
	msh_parse.c \
	msh_pre_heredoc.c \
	msh_heredoc.c \
	msh_pipeline.c \
	msh_parse_to_exit_status.c \
	msh_pipeline_stage.c \
	msh_preparse_stage.c \
	msh_lex_stage.c \
	msh_heredoc_stage.c \
	msh_cleanup.c \
	$(SH_SRC) \
	$(UTILS_SRC) \
	$(PIPELINE_SRC) \
	$(ENV_SRC) \
	$(ERRORS_SRC) \
	$(EXECUTOR_SRC) \
	$(PARSE_SRC) \
	$(HEREDOC_SRC) \
	$(EXPANSION_SRC)

OBJ := $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIBFT_DIR) -lft $(LDLIBS_READLINE) -o $(NAME)

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