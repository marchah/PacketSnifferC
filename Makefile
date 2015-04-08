NAME=	sniffer

SRC=	main.c

CC=	gcc

CFLAGS= -W -Wall -Wextra -Werror -ansi -pedantic

OBJ=	$(SRC:.c=.o)

all:	$(NAME)

$(NAME):	$(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(CFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re:     fclean all

.PHONY: all clean fclean re
