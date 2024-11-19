# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/04 18:23:01 by ele-borg          #+#    #+#              #
#    Updated: 2024/11/17 19:07:34 by ele-borg         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#variables

SRC_DIR = sources
OBJ_DIR = objets

SRC_FILES = ${SRC_DIR}/main.c \
			${SRC_DIR}/libft.c \
			${SRC_DIR}/ft_split.c \
			${SRC_DIR}/ft_split_utils.c \
			${SRC_DIR}/signal.c \
			${SRC_DIR}/parsing.c \
			${SRC_DIR}/lst_functions_parta.c \
			${SRC_DIR}/lst_functions_partb.c \
			${SRC_DIR}/ft_tokenize.c \

OBJ_FILES = $(OBJ_DIR)/main.o \
            ${OBJ_DIR}/libft.o \
			${OBJ_DIR}/ft_split.o \
			${OBJ_DIR}/ft_split_utils.o \
			${OBJ_DIR}/signal.o \
			${OBJ_DIR}/parsing.o \
			${OBJ_DIR}/lst_functions_parta.o \
			${OBJ_DIR}/lst_functions_partb.o \
			${OBJ_DIR}/ft_tokenize.o \

NAME = minishell

FLAGS_C = -Wall -Wextra -Werror -g3 -I.

FLAGS_L = -lreadline

CC = cc

#rules

all: ${NAME} 

${NAME}: ${OBJ_FILES}
	${CC} ${FLAGS_L} ${OBJ_FILES} -o ${NAME}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c
	@mkdir -p ${OBJ_DIR}
	$(CC) -c $(FLAGS_C) $< -o $@

clean: 
	rm  -f ${OBJ_FILES}

fclean: clean
	rm -f ${NAME}

re: fclean all

reclean : all clean

.PHONY: all clean fclean re