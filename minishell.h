/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:29:10 by aisidore          #+#    #+#             */
/*   Updated: 2025/01/27 18:18:06 by ele-borg         ###   ########.fr       */
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
# include <errno.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <limits.h>
# include <termios.h>
# include <sys/ioctl.h>
//# include <asm/signal.h> //pour eviter le rouge sa chez Erika
# include "gc.h"

# define CRESET		"\x1b[0m"
# define CRED		"\x1b[31m"
# define CGREEN		"\x1b[32m"
# define CYELLOW		"\x1b[33m"
# define CBLUE		"\x1b[34m"
# define CMAGENTA	"\x1b[35m"
# define CCYAN		"\x1b[36m"

# define UN_DOLLAR	"minishell: syntax error near unexpected token `$'\n"
# define UN_PIPE		"minishell: syntax error near unexpected token `|'\n"
# define UN_TOKEN	"minishell: syntax error near unexpected token `%s'\n"
# define UN_NLINE	"minishell: syntax error near unexpected token `newline'\n"

# define 	INPUT		1
# define	HEREDOC		2
# define 	TRUNC		3
# define 	APPEND		4
# define	PIPE		5
# define	CMD			6
# define 	ARG			7

# define	PERROR		101
# define	NO_PERROR	102

# define	HERE		103
# define	NO_HERE		104

# define	NO_TRY_OPEN	-2
# define	ERROR_OPEN	-1
# define	CLOSED		-3

# define	TRUE		1
# define	FALSE		0

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

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
	int		active;
}	t_cmd;

typedef struct	s_element
{
	char	*line;
	char	**arr;
	char	**env;
	char	**mypaths;
	t_cmd	*lst; // il s'agit de la liste des commandes a executer
	int		nb_cmd;
	int		*pid_arr;
	int		**pipes;
	char	*error;
	int		child_to_wait;
	int		line_valid;
	//int		err;
	char	*exit_status;
}	t_element;

typedef struct s_built
{
	t_element	*elements;
	char		**cmd;
}	t_built;

typedef struct	s_var
{
	int	j;
	int	k;
}	t_var;

typedef struct	s_vars
{
	int		fd;
	char	*line;
}	t_vars;

typedef struct s_env
{
	char         *name;
	char         *key;
	struct s_env *next;
}   t_env;

extern volatile sig_atomic_t g_signal;

extern rl_hook_func_t *rl_event_hook;

//parsing.c
void	ft_deldollar(char *input);
int		ft_istok_(char *av2);
void	ft_ft(t_element *elements, t_gc *gc);

//ft_split.c
char	**ft_split(char const *str, int sq, int dq, t_gc *gc);

//ft_exit.c
int ft_exit(t_built *built, t_gc *gc);

//ft_echo.c
void	ft_echo(char **cmd, t_gc *gc);

//ft_split_utils.c
size_t	ft_count(const char *str, const char sep);
char	*ft_eachword(const char *str, int fidx, int end, t_gc *gc);
void	*ft_freesplit(char **tab, size_t n);

//libft_e.c
int		ft_arr_size(char **tab);
char	*ft_strdup(const char *s1, t_gc *gc);
char	*ft_substr(char const *s, unsigned int start, size_t len, t_gc *gc);
void	ft_putstr_fd(char *s, int fd);
int		ft_atoi(const char *str);

//libft_a.c
size_t	ft_strlen(const char *str);
void	*ft_calloc(size_t n, size_t sizof);
int		ft_strcmp(char *s1, char *s2);
int		ft_strncmp(const char *str1, const char *str2, size_t n);//ATTENTION Erika a aussi un ft_strncmp pas code exactement comme moi
int     ft_isalnum(int c);

//libft_abis.c
char	*ft_strdup_(char const *str, t_gc *gc);
char    *ft_strncpy(char *dest, const char *src, size_t n);
char    *ft_strcat(char *dst, const char *src);
int     ft_isalpha(int c);
char	*ft_strjoin_(char *str1, char *str2, t_gc *gc);

//error.c
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
void	ft_buff_error(char *str, t_element *elements, t_gc *gc);
void	ft_write_error(t_element *elements, t_gc *gc);

//signal.c
void	ft_handle_signal(int process, t_gc *gc);
void	ft_interactive_signal(t_gc *gc);
void	ft_heredoc_signal(t_gc *gc);
void	ft_ignore_signal(t_gc *gc);
void	ft_exec_signal(t_gc *gc);
void	reset_signal_status(void);

//init.c
void		ft_welcome(void);
void		ft_cpy_env(t_element *elements, char **env, t_gc *gc);
t_element	*ft_init_struct(t_gc *gc);
//void		ft_error_exit(char *s, int	i, int type);

//ft_tokenize.c
char	*ft_tokenize(char *av2, t_gc *gc, t_element *elements);

//ft_ifexpand.c
char	*ft_ifexpand(char *result, int sq, int dq, t_element *elements);

//ft_ambig.c
void	ft_ambig(char *result_k, int *k, t_element *elements);

//ft_getenvv.c
char	*ft_getenvv(char *result, int *k, char *tmp, t_element *elements);
char	*ft_itoa(int nb, t_gc *gc);

//str_manager.c
char	*ft_insert(char *result, int k, char c);
char	*ft_erase(char *result, int tmp_k);
void	ft_erase_substr(char *result, int *k, char *tmp);
void	ft_modifquote_(char const *str, int *sq, int *dq, int *i);

//ft_concat.c
char	*ft_concat(char *result_i, int k, int sq, int dq);

//redir_chain.c
t_file	*create_redir(char **tab, int i, int last_i, t_gc *gc);

//lexing.c
void	hedge_case_1(char **tab, t_element *elements, t_gc *gc);
int		ft_istok_2(char *av2);
void	lexing(char **tab, t_cmd **lst, t_element *elements, t_gc *gc);

//create_chain.c
bool	ft_is_redir(char *s);
bool	ft_is_str(char *s);
void	create_chain(char **tab, int i, int last_i, t_cmd **lst, t_gc *gc);

// redir_open_parta.c
void	ft_fd_open(t_cmd *node, t_element *elements, t_gc *gc, int flag);
void	handle_redir(t_cmd **lst, t_element *elements, t_gc *gc);
void	ft_handle_in(t_cmd *node, t_file *redir, t_element *elements, t_gc *gc);
void	ft_handle_out(t_cmd *node, t_file *redir, t_element *elements, t_gc *gc);

// redir_open_partb.c
void	ft_error_out(char *name, t_element *elements, t_gc *gc);
void	ft_handle_no_here_out(t_cmd *node, t_file *redir, t_element *elements, t_gc *gc);
//int	ft_open_heredoc_error(char *del, t_element *elements, t_gc *gc);

//heredoc.c
int	ft_open_heredoc(char *del, t_element *elements, t_gc *gc);
int	ft_open_heredoc_error(char *del, t_element *elements, t_gc *gc);

// cmd_arr.c
int		nb_arg(char **tab, int i, int last_i);
char	**cmd_arr(char **tab, int i, int last_i, t_gc *gc);
void	ft_fill_arr(char **arr, char **tab, int i, int last_i, t_gc *gc);

// split_paths.c
void	ft_handle_path(t_element *elements, t_gc *gc);

//fill_arrays.c
void	ft_fill_arrays(t_element *elements, t_gc *gc);

//pipe_handle.c
void	pipe_creation(t_element *elements, t_gc *gc);
void	close_pipes(t_element *elements);
void	part_close(t_element *elements, int k);
void	wait_for_children(t_element *elements, t_gc *gc);
void	free_std(void);

//pipe_closing.c
void	first_cmd_with_valid_infile(t_element *elements, t_cmd *cmd, t_gc *gc);
void	last_cmd_with_valid_outfile(t_element *elements, t_cmd *cmd, t_gc *gc);
void	dup_and_close_read_pipe(int k, t_element *elements, t_cmd *cmd, t_gc *gc);
void	dup_and_close_write_pipe(int k, t_element *elements, t_cmd *cmd, t_gc *gc);
void	all_cases(int i, t_element *elements, t_cmd *cmd, t_gc *gc);

//built_in.c
int		is_built_in(char *cmd);
void	ft_built_in(t_element *elements, char **cmd, t_gc *gc);
void	built_in_no_child(t_element *elements, t_gc *gc);
void	close_other_redir(int i, t_element *elements);

//child_creation.c
void	uniq_case(t_element *elements, t_cmd *cmd, t_gc *gc);
void	child_process(int i, t_element *elements, t_cmd *cmd, t_gc *gc);
int		no_child_events(t_element *elements, t_gc *gc, t_cmd *current);
void	hedge_child_cases(t_element *elements, t_gc *gc, t_cmd	*current);
void	child_creation(t_element *elements, t_gc *gc);

//execution.c
void	write_all_err_mess(char *str1, char *str2, t_element *elements, t_gc *gc);
void	path_abs(char **cmd, t_element *elements, int i, t_gc *gc);
void	path_relat(char **cmd, t_element *elements, int i, t_gc *gc);
void	exec_command(t_element *elements, t_gc *gc, int i);

//env_manager.c
char	**ft_ltoa(t_env *head, t_gc *gc);
void    ft_freetab(char **array);
int     ft_exparser(char *name_key);
void    ft_env(char **array, char **cmds, t_gc *gc);

//ft_export.c
void ft_printexport(const t_env *head);
// void ft_freelexport(t_env *head);
void ft_adder(t_env **head, char *str, t_gc *gc);
char **ft_export(t_element *element, char **argv, t_gc *gc);

//ft_export_utils.c
t_env    *ft_envnode(char *name, char *key, t_gc *gc);
t_env    *ft_addenvnode(t_env *head, char *name, char *key, t_gc *gc);
char     *ft_cut(const char *src, char delim, int is_end, t_gc *gc);
void     ft_swapnodes(t_env *node1, t_env *node2);
void     ft_bbsort(t_env *head);

//ft_unset.c
void	ft_unset(t_element *element, char **argv, t_gc *gc);

//cd_pwd.c
void	ft_pwd(t_element *elements, t_gc *gc);
void	ft_cd(t_built *built, t_gc *gc);

// pour test
void print_cmd_list(t_cmd *cmd_list);
void check_fds(const char *context);

#endif
