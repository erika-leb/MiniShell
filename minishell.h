/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:29:10 by aisidore          #+#    #+#             */
/*   Updated: 2024/11/21 19:12:54 by ele-borg         ###   ########.fr       */
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
# include <stdbool.h>
//# include <asm/signal.h> //pour eviter le rouge sa chez Erika
# include "gc.h"

#define CRESET		"\x1b[0m"
#define CRED		"\x1b[31m"
#define CGREEN		"\x1b[32m"
#define CYELLOW		"\x1b[33m"
#define CBLUE		"\x1b[34m"
#define CMAGENTA	"\x1b[35m"
#define CCYAN		"\x1b[36m"

# define 	INPUT	1
# define	HEREDOC	2
# define 	TRUNC	3
# define 	APPEND	4
# define	PIPE	5
# define	CMD		6
# define 	ARG		7

# define	PERROR		101
# define	NO_PERROR	102

typedef struct	s_element
{
	char	*line;
	char	**arr;
}	t_element;

// typedef struct s_token
// {
// 	char			*name;
// 	int				token;
// 	struct s_token *next;
// }	t_token;

typedef struct s_file
{
	char			*name;
	int				token;
	struct s_file	*next;
}	t_file;


typedef struct s_cmd
{
	char	**cmd;
	t_file	*redir;
	int		fd_in;
	int		fd_out;
	//char	*path; //pas encore ?
	struct s_cmd *next;
}	t_cmd;

//ft_split.c
char	**ft_split(char const *str, char sep, int sq, int dq);

//ft_split_utils.c
size_t	ft_count(const char *str, const char sep);
char	*ft_eachword(const char *str, int fidx, int end);
void	*ft_freesplit(char **tab, size_t n);

//libft.c
void	*ft_calloc(size_t n, size_t sizof);
size_t	ft_strlen(const char *str);
int		ft_strcmp(char *s1, char *s2);
int	ft_arr_size(char **tab);

//init.c
void	ft_signal_handle(t_gc *gc);
void		ft_welcome(void);
t_element	*ft_init_struct(void);
//void		ft_error_exit(char *s, int	i, int type);

////lst_functions.c
//t_cmd	*ft_lstnew(char *str, int cmd);
//void	ft_lstadd_front(t_cmd **lst, t_cmd *new);
//int		ft_lstsize(t_cmd **lst);
//t_cmd	*ft_lstlast(t_cmd *lst);
//void	ft_lstadd_back(t_cmd **lst, t_cmd *new);
//void	ft_lstclear(t_cmd *lst);

//ft_tokenize.c
char	*ft_tokenize(char *av2);

#endif
