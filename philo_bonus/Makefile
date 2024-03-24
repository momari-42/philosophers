CFLALGS = -Wall -Wextra -Werror
#  -fsanitize=thread 
NAME = philo_bonus
CC = cc
SRC = philosophers_bonus.c src/ft_atoi.c

OBJS = $(SRC:.c=.o)

all: $(NAME)

%.o: %.c philosophers_bonus.h
	$(CC) $(CFLALGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLALGS) $(OBJS) -o $(NAME) 

clean:
	rm -rf $(OBJS) $(BOBJS)

fclean: clean
	rm -rf $(NAME) $(BOBJS)

re: fclean all