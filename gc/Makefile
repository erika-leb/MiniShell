# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/04 18:23:01 by ele-borg          #+#    #+#              #
#    Updated: 2024/11/22 16:07:48 by ele-borg         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#variables

SRC_FILES =	gc_cleanup.c \
			gc_malloc.c \
			gc_realloc.c \
			gc_remove.c \

FLAGS_C = -Wall -Wextra -Werror -g3 -I.

CC = cc

#rules

all: $(OBJ_FILES)

%.o: %.c
	$(CC) -c $(FLAGS_C) $< -o $@

clean:
	rm  -f ${OBJ_FILES}

fclean: clean
	rm -f ${NAME}

re: fclean all

reclean : all clean

.PHONY: all clean fclean re
