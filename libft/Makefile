CFLAGS = -Wall -Wextra -Werror
CC     = cc
NAME   = libft.a

GNL_DIR    := get_next_line
PRINTF_DIR := printf


SRCS := \
ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c \
ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_utoa.c \
ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c \
ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c \
ft_split.c ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c \
ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c \
ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c \
ft_satoi.c ft_strcmp.c ft_isspace.c ft_satoll.c


GNL_SRC := get_next_line.c


PRINTF_SRC := $(addprefix $(PRINTF_DIR)/, ft_printf.c ft_functions.c ft_eprintf.c)

OBJ        := $(SRCS:.c=.o)
GNL_OBJ    := $(addprefix $(GNL_DIR)/,$(GNL_SRC:.c=.o))
PRINTF_OBJ := $(PRINTF_SRC:.c=.o)

AR = ar rcs
RM = rm -f

all: $(NAME)

$(NAME): $(OBJ) $(GNL_OBJ) $(PRINTF_OBJ)
	$(AR) $(NAME) $^


%.o: %.c libft.h
	$(CC) $(CFLAGS) -I . -c $< -o $@


$(GNL_DIR)/%.o: $(GNL_DIR)/%.c libft.h
	$(CC) $(CFLAGS) -I . -c $< -o $@


$(PRINTF_DIR)/%.o: $(PRINTF_DIR)/%.c libft.h
	$(CC) $(CFLAGS) -I . -I $(PRINTF_DIR) -c $< -o $@

clean:
	$(RM) $(OBJ) $(GNL_OBJ) $(PRINTF_OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
