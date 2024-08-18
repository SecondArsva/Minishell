NAME = minishell

CC = cc -Wall -Wextra -Werror
LDFLAGS = -lreadline
RM = rm -f

LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)libft.a

SRC =	src/main.c			\
		src/safe_alloc.c	\
		src/tokenizer.c		\
		src/init_env.c		\
		src/expander.c		\
		
OBJS = $(SRC:%.c=%.o)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(OBJS) $(LIBFT) -o $(NAME) $(LDFLAGS)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	$(CC) -c $< -o $@

clean:
	$(RM) $(OBJS)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	$(RM) $(NAME)
	$(MAKE) fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re
