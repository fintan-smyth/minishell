# Compiler
CC = cc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -g3

SRC_DIR := ./src

BUILD_DIR:= ./build

SRC = $(SRC_DIR)/main.c \
	  $(SRC_DIR)/entries.c \
	  $(SRC_DIR)/exec.c \
	  $(SRC_DIR)/environ.c \
	  $(SRC_DIR)/builtins/cd.c \
	  $(SRC_DIR)/builtins/env.c \
	  $(SRC_DIR)/builtins/pwd.c \
	  $(SRC_DIR)/builtins/echo.c \
	  $(SRC_DIR)/parsing/tokenise.c \
	  $(SRC_DIR)/parsing/expansion.c \
	  $(SRC_DIR)/parsing/strip_quotes.c \
	  $(SRC_DIR)/parsing/commands.c \
	  $(SRC_DIR)/parsing/redirection.c \

OBJ = $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SRC:.c=.o))

NAME = minishell

LIBFT = libft/libft.a

mand: $(NAME)

# bonus: $(NAME_BONUS)

all: mand bonus

$(NAME): $(LIBFT) $(BUILD_DIR) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -L ./libft -lft -lreadline

$(OBJ): $(BUILD_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(LIBFT):
	make -C libft/ bonus

$(BUILD_DIR):
	mkdir -p $@/builtins
	mkdir -p $@/parsing

clean:
	rm -rf ./build/
	make -C libft/ clean

fclean: clean
	rm -rf $(NAME) # $(NAME_BONUS)
	make -C libft/ fclean

re: fclean all
.PHONY: all clean fclean re mand bonus
