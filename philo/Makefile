CFLALGS = -Wall -Wextra -Werror -pthread 
NAME = philo
CC = cc
SRC = philosophers.c src/ft_atoi.c

OBJS = $(SRC:.c=.o)

all: $(NAME)

%.o: %.c philosophers.h
	$(CC) $(CFLALGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLALGS) $(OBJS) -o $(NAME) 

clean:
	rm -rf $(OBJS) $(BOBJS)

fclean: clean
	rm -rf $(NAME) $(BOBJS)

re: fclean all