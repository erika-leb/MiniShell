/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:29:10 by aisidore          #+#    #+#             */
/*   Updated: 2025/01/07 16:09:36 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>//printf
# include <stdlib.h>//free, ...
# include <stdint.h>//SIZE_MAX
# include <signal.h>//sigaction
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
#include <limits.h>//Car uniquement utilisé pour ft_itoa (potentiellement useless)

# define COLOR_RESET   "\x1b[0m"
# define COLOR_RED     "\x1b[31m"
# define COLOR_GREEN   "\x1b[32m"
# define COLOR_YELLOW  "\x1b[33m"
# define COLOR_BLUE    "\x1b[34m"
# define COLOR_MAGENTA "\x1b[35m"
# define COLOR_CYAN    "\x1b[36m"

//Useless ?? A quoi ca sert ?
# define UN_TOKEN "bash: syntax error"

//ft_getenvv.c
char	*ft_getenvv(char *result, int *k, char *tmp);

//parsing.c
void	ft_deldollar(char *input);
int     ft_istok_(char *av2);
void	ft_ft(char *line, char **result, int i, int go);

//ft_split.c
char	**ft_split(char const *str, int sq, int dq);

//split_utils.c
size_t	ft_count(const char *str, const char sep);
char	*ft_eachword(const char *str, int fidx, int end);
void	*ft_freesplit(char **tab, size_t n);

//libft_a.c
void	*ft_calloc(size_t n, size_t sizof);
size_t	ft_strlen(const char *str);
int		ft_strcmp(char *s1, char *s2);
int		ft_strncmp(const char *str1, const char *str2, size_t n);//ATTENTION Erika a aussi un ft_strncmp pas code exactement comme moi
int     ft_isalnum(int c);

//ft_tokenize.c
char	*ft_tokenize(char *av2);

//ft_ifexpand
char	*ft_ifexpand(char *result, int sq, int dq);

//ft_ambig
void	ft_ambig(char *result_k, int *k);

//str_manager.c
char *ft_insert(char *result, int k, char c);
char *ft_erase(char *result, int tmp_k);
void ft_erase_substr(char *result, int *k, char *tmp);
void ft_modifquote_(char const *str, int *sq, int *dq, int *i);

//ft_concat.c
char	*ft_concat(char *result_i, int k, int sq, int dq);

#endif
