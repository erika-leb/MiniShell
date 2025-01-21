# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/04 18:23:01 by ele-borg          #+#    #+#              #
#    Updated: 2025/01/21 15:41:23 by ele-borg         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#variables

SRC_DIR = sources
OBJ_DIR = objets
GC_DIR = gc

SRC_FILES = ${SRC_DIR}/main.c \
			${SRC_DIR}/parsing.c \
			${GC_DIR}/gc_cleanup.c \
			${GC_DIR}/gc_malloc.c \
			${GC_DIR}/gc_realloc.c \
			${GC_DIR}/gc_remove.c \
			${SRC_DIR}/libft_e.c \
			${SRC_DIR}/libft_a.c \
			${SRC_DIR}/libft_abis.c \
			${SRC_DIR}/ft_split.c \
			${SRC_DIR}/ft_split_utils.c \
			${SRC_DIR}/init.c \
			${SRC_DIR}/lexing.c \
			${SRC_DIR}/ft_tokenize.c \
			${SRC_DIR}/redir_chain.c \
			${SRC_DIR}/ft_ifexpand.c \
			${SRC_DIR}/ft_ambig.c \
			${SRC_DIR}/ft_getenvv.c \
			${SRC_DIR}/str_manager.c \
			${SRC_DIR}/ft_concat.c \
			${SRC_DIR}/redir_open_parta.c \
			${SRC_DIR}/redir_open_partb.c \
			${SRC_DIR}/cmd_arr.c \
			${SRC_DIR}/split_path.c \
			${SRC_DIR}/child_creation.c \
			${SRC_DIR}/fill_arrays.c \
			${SRC_DIR}/execution.c \
			${SRC_DIR}/pipe_handle.c \
			${SRC_DIR}/error_message.c \
			${SRC_DIR}/signal.c \
			${SRC_DIR}/ft_exit.c \
			${SRC_DIR}/ft_echo.c \
			${SRC_DIR}/env_manager.c \
			${SRC_DIR}/ft_export.c \
			${SRC_DIR}/ft_export_utils.c \
			${SRC_DIR}/ft_unset.c \

OBJ_FILES =	$(OBJ_DIR)/main.o \
			$(OBJ_DIR)/parsing.o \
			${OBJ_DIR}/gc_cleanup.o \
			${OBJ_DIR}/gc_malloc.o \
			${OBJ_DIR}/gc_realloc.o \
			${OBJ_DIR}/gc_remove.o \
			${OBJ_DIR}/libft_e.o \
			${OBJ_DIR}/libft_a.o \
			${OBJ_DIR}/libft_abis.o \
			${OBJ_DIR}/ft_split.o \
			${OBJ_DIR}/ft_split_utils.o \
			${OBJ_DIR}/init.o \
			${OBJ_DIR}/lexing.o \
			${OBJ_DIR}/ft_tokenize.o \
			${OBJ_DIR}/redir_chain.o \
			${OBJ_DIR}/ft_ifexpand.o \
			${OBJ_DIR}/ft_ambig.o \
			${OBJ_DIR}/ft_getenvv.o \
			${OBJ_DIR}/str_manager.o \
			${OBJ_DIR}/ft_concat.o \
			${OBJ_DIR}/redir_open_parta.o \
			${OBJ_DIR}/redir_open_partb.o \
			${OBJ_DIR}/cmd_arr.o \
			${OBJ_DIR}/split_path.o \
			${OBJ_DIR}/child_creation.o \
			${OBJ_DIR}/fill_arrays.o \
			${OBJ_DIR}/execution.o \
			${OBJ_DIR}/pipe_handle.o \
			${OBJ_DIR}/error_message.o \
			${OBJ_DIR}/signal.o \
			${OBJ_DIR}/ft_exit.o \
			${OBJ_DIR}/ft_echo.o \
			${OBJ_DIR}/env_manager.o \
			${OBJ_DIR}/ft_export.o \
			${OBJ_DIR}/ft_export_utils.o \
			${OBJ_DIR}/ft_unset.o \

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
