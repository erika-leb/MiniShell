# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/04 18:23:01 by ele-borg          #+#    #+#              #
#    Updated: 2025/01/07 18:20:18 by aisidore         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#variables

SRC_DIR = sources
OBJ_DIR = objets

SRC_FILES = ${SRC_DIR}/main.c \
			${SRC_DIR}/libft_a.c \
			${SRC_DIR}/ft_getenvv.c \
			${SRC_DIR}/parsing.c \
			${SRC_DIR}/ft_split.c \
			${SRC_DIR}/split_utils.c \
			${SRC_DIR}/ft_tokenize.c \
			${SRC_DIR}/ft_ifexpand.c \
			${SRC_DIR}/ft_ambig.c \
			${SRC_DIR}/str_manager.c \
			${SRC_DIR}/ft_concat.c#backslash
#${SRC_DIR}/ft_exit.c

OBJ_FILES = $(OBJ_DIR)/main.o \
            ${OBJ_DIR}/libft_a.o \
			${OBJ_DIR}/ft_getenvv.o \
			${OBJ_DIR}/parsing.o \
			${OBJ_DIR}/ft_split.o \
			${OBJ_DIR}/split_utils.o \
			${OBJ_DIR}/ft_tokenize.o \
			${OBJ_DIR}/ft_ifexpand.o \
			${OBJ_DIR}/ft_ambig.o \
			${OBJ_DIR}/str_manager.o \
			${OBJ_DIR}/ft_concat.o#backslash
#${OBJ_DIR}/ft_exit.o

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