/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:29:10 by aisidore          #+#    #+#             */
/*   Updated: 2024/11/29 13:47:08 by ele-borg         ###   ########.fr       */
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
# include <fcntl.h>
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

# define	HERE		103
# define	NO_HERE		104

# define	NO_TRY_OPEN	-2
# define	ERROR_OPEN	-1
# define	CLOSED		-3

# define UN_TOKEN "bash: syntax error"

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
	int		code_status; // a recevoir dans le pipe du dernier enfant
}	t_cmd;

typedef struct	s_element
{
	char	*line;
	char	**arr;
	char	**env;
	char	**mypaths;
	t_cmd	*lst;
}	t_element;

typedef struct	s_var
{
	int	j;
	int	k;
}	t_var;

//ft_split.c
char	**ft_split(char const *str, int sq, int dq, t_gc *gc);

//ft_split_utils.c
size_t	ft_count(const char *str, const char sep);
char	*ft_eachword(const char *str, int fidx, int end, t_gc *gc);
void	*ft_freesplit(char **tab, size_t n);

//libft.c
void	*ft_calloc(size_t n, size_t sizof); // A SUPPRIMER
size_t	ft_strlen(const char *str);
int		ft_strcmp(char *s1, char *s2);
int		ft_arr_size(char **tab);
char	*ft_strdup(const char *s1, t_gc *gc);
int		ft_isalnum(int c);
char	*ft_substr(char const *s, unsigned int start, size_t len, t_gc *gc);
void	ft_putstr_fd(char *s, int fd);

//init.c
void		ft_signal_handle(t_gc *gc);
void		ft_welcome(void);
t_element	*ft_init_struct(t_gc *gc);
//void		ft_error_exit(char *s, int	i, int type);

//ft_tokenize.c
char	*ft_tokenize(char *av2);
//char	*ft_concat(char *result, int sq, int dq);//unused pour l'instant

//ft_ifexpand
char	*ft_ifexpand(char *result);

//man_large_str.c
void 	ft_insert(char *result, int k, char c);
char 	*ft_erase(char *result, int tmp_k);
void 	ft_erase_substr(char *result, int *k, char *tmp);
void 	ft_modifquote_(char const *str, int *sq, int *dq, int *i);

//ft_concat.c
char	*ft_concat(char *result_i, int k, int sq, int dq);

//redir_chain.c
t_file	*create_redir(char **tab, int i, int last_i, t_gc *gc);

//lexing.c
bool	ft_is_redir(char *s);
bool	ft_is_str(char *s);
void	lexing(char **tab, t_cmd **lst, t_gc *gc);
void	create_chain(char **tab, int i, int last_i, t_cmd **lst, t_gc *gc);

// redir_open_parta.c
void	ft_fd_open(t_cmd *node);
void	handle_redir(t_cmd **lst);

// redir_open_partb.c
int		ft_open_heredoc(char *del);
void	ft_handle_in(t_cmd *node, t_file *redir);
void	ft_handle_no_here_out(t_cmd *node, t_file *redir);
void	ft_handle_out(t_cmd *node, t_file *redir);

// cmd_arr.c
int		nb_arg(char **tab, int i, int last_i);
char	**cmd_arr(char **tab, int i, int last_i, t_gc *gc);
void	ft_fill_arr(char **arr, char **tab, int i, int last_i, t_gc *gc);

// split_paths.c
void	ft_handle_path(t_element *elements, t_gc *gc);

#endif
