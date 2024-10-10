# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shuwang <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/28 14:01:02 by shuwang           #+#    #+#              #
#    Updated: 2024/09/28 14:01:04 by shuwang          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc

CFLAGS = -Wall -Wextra -Werror 

SRCS = main.c check_input.c philo_utils.c take_forks.c philo_action.c monitor.c philo_func.c print_msg.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all clean fclean re
