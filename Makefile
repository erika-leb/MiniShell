# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/04 18:23:01 by ele-borg          #+#    #+#              #
#    Updated: 2024/11/27 14:37:52 by ele-borg         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#variables

SRC_DIR = sources
OBJ_DIR = objets
GC_DIR = gc

SRC_FILES = ${SRC_DIR}/main.c \
			${GC_DIR}/gc_cleanup.c \
			${GC_DIR}/gc_malloc.c \
			${GC_DIR}/gc_realloc.c \
			${GC_DIR}/gc_remove.c \
			${SRC_DIR}/libft.c \
			${SRC_DIR}/ft_split.c \
			${SRC_DIR}/ft_split_utils.c \
			${SRC_DIR}/init.c \
			${SRC_DIR}/lexing.c \
			${SRC_DIR}/ft_tokenize.c \
			${SRC_DIR}/redir_chain.c \
			${SRC_DIR}/ft_ifexpand.c \
			${SRC_DIR}/man_large_str.c \
			${SRC_DIR}/ft_concat.c \
			${SRC_DIR}/redir_open.c

OBJ_FILES =	$(OBJ_DIR)/main.o \
			${OBJ_DIR}/gc_cleanup.o \
			${OBJ_DIR}/gc_malloc.o \
			${OBJ_DIR}/gc_realloc.o \
			${OBJ_DIR}/gc_remove.o \
			${OBJ_DIR}/libft.o \
			${OBJ_DIR}/ft_split.o \
			${OBJ_DIR}/ft_split_utils.o \
			${OBJ_DIR}/init.o \
			${OBJ_DIR}/lexing.o \
			${OBJ_DIR}/ft_tokenize.o \
			${OBJ_DIR}/redir_chain.o \
			${OBJ_DIR}/ft_ifexpand.o \
			${OBJ_DIR}/man_large_str.o \
			${OBJ_DIR}/ft_concat.o \
			${OBJ_DIR}/redir_open.o \

NAME = minishell

FLAGS_C = -Wall -Wextra -Werror -g3 -I. -Igc

FLAGS_L = -lreadline

CC = cc

#rules

all: ${NAME}

${NAME}: ${OBJ_FILES}
	${CC} ${FLAGS_L} ${OBJ_FILES} -o ${NAME}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c
	@mkdir -p ${OBJ_DIR}
	$(CC) -c $(FLAGS_C) $< -o $@

${OBJ_DIR}/%.o: ${GC_DIR}/%.c
	@mkdir -p ${OBJ_DIR}
	$(CC) -c $(FLAGS_C) $< -o $@

GC_OBJ_FILES:
	$(MAKE) -C ${GC_DIR}

clean:
	rm  -f ${OBJ_FILES}

fclean: clean
	rm -f ${NAME}

re: fclean all

reclean : all clean

.PHONY: all clean fclean re