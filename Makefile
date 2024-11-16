# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/04 18:23:01 by ele-borg          #+#    #+#              #
#    Updated: 2024/11/16 16:54:08 by aisidore         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#variables

SRC_DIR = sources
OBJ_DIR = objets

SRC_FILES = ${SRC_DIR}/main.c \
			${SRC_DIR}/libft.c \
			${SRC_DIR}/ft_split.c \
			${SRC_DIR}/ft_split_utils.c

OBJ_FILES = $(OBJ_DIR)/main.o \
            ${OBJ_DIR}/libft.o \
			${OBJ_DIR}/ft_split.o \
			${OBJ_DIR}/ft_split_utils.o

NAME = minishell

FLAGS = -Wall -Wextra -Werror -g3 -I.

CC = cc

#rules

all: ${NAME} 

${NAME}: ${OBJ_FILES}
	${CC} ${FLAGS} ${OBJ_FILES} -o ${NAME}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c
	@mkdir -p ${OBJ_DIR}
	$(CC) -c $(FLAGS) $< -o $@

clean: 
	rm  -f ${OBJ_FILES}

fclean: clean
	rm -f ${NAME}

re: fclean all

reclean : all clean

.PHONY: all clean fclean re