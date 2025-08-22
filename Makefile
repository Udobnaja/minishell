NAME := minishell

CC := cc

CFLAGS := -Wall -Wextra -Werror -I.  

SRC := main.c

OBJ := $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -lreadline