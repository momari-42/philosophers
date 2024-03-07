CFLALGS = -Wall -Wextra -Werror
NAME = philo
CC = cc
SRC = philosophers.c src/ft_atoi.c 
# src/ft_lstadd_back.c src/ft_lstnew.c


# BSRC = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c ft_lstadd_back_bonus.c \
# 		 ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c

OBJS = $(SRC:.c=.o)
# BOBJS = $(BSRC:.c=.o)

all: $(NAME)

%.o: %.c philosophers.h
	$(CC) $(CFLALGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLALGS) $(OBJS) -o $(NAME) -fsanitize=address

# bonus: $(BOBJS)
# 	ar rcs $(NAME) $(BOBJS)

clean:
	rm -rf $(OBJS) $(BOBJS)

fclean: clean
	rm -rf $(NAME) $(BOBJS)

re: fclean all