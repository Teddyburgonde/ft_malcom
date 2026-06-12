NAME		= ft_malcolm

CC			= cc
CFLAGS		= -Wall -Wextra -Werror

INC_DIR		= includes
SRC_DIR		= srcs
OBJ_DIR		= objs

SRCS		= main.c \
			  parsing.c \
			  signal.c \
			  socket.c \
			  network.c \
			  arp.c \
			  dns.c

OBJS		= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
