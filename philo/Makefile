# Compiler and flags
CC			= cc
CFLAGS = -Wall -Wextra -Werror -Iincludes -g3
DEBUGFLAGS	= -g

# Directories
SRC_DIR		= ./src
OBJ_DIR		= ./.obj
INC_DIR		= ./includes

# Sources
SRC_FILES	= $(shell find $(SRC_DIR) -name '*.c')

MAIN_SRC	= main.c

SRCS		= $(SRC_FILES) $(MAIN_SRC)

# Object files with directory structure included (to avoid name conflicts)
OBJS		= $(SRCS:%.c=$(OBJ_DIR)/%.o)

# Name of the program
NAME		= philo

# Includes
INCLUDES	= -I$(INC_DIR)

# Linker flags for readline
LDFLAGS		= -lreadline

# Phony targets
.PHONY: all clean fclean re debug

# Default rule
all: $(NAME)

# Link the program, adding the readline library
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

# Compile object files, ensuring directory structure in obj/
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Debug rule
debug: CFLAGS += $(DEBUGFLAGS)
debug: re

# Clean object files
clean:
	rm -rf $(OBJ_DIR)

# Clean object files and the binary
fclean: clean
	rm -f $(NAME)

# Recompile everything
re: fclean all

