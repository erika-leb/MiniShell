/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:29:10 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/14 16:54:26 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "gc.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

# define CRESET "\x1b[0m"
# define CRED "\x1b[31m"
# define CGREEN "\x1b[32m"
# define CYELLOW "\x1b[33m"
# define CBLUE "\x1b[34m"
# define CMAGENTA "\x1b[35m"
# define CCYAN "\x1b[36m"

# define UN_DOLLAR "minishell: syntax error near unexpected token `$'\n"
# define UN_PIPE "minishell: syntax error near unexpected token `|'\n"
# define UN_TOKEN "minishell: syntax error near unexpected token `%s'\n"
# define UN_NLINE "minishell: syntax error near unexpected token `newline'\n"

# define INPUT 1
# define HEREDOC 2
# define TRUNC 3
# define APPEND 4
# define PIPE 5
# define CMD 6
# define ARG 7

# define PERROR 101
# define NO_PERROR 102

# define HERE 103
# define NO_HERE 104

# define NO_TRY_OPEN -2
# define ERROR_OPEN -1
# define CLOSED -3

# define TRUE 1
# define FALSE 0

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
	char			**cmd;
	t_file			*redir;
	int				fd_in;
	int				fd_out;
	struct s_cmd	*next;
	int				active;
}	t_cmd;

typedef struct s_element
{
	char			*line;
	char			**arr;
	char			**env;
	char			**mypaths;
	t_cmd			*lst;
	int				nb_cmd;
	int				*pid_arr;
	int				**pipes;
	char			*error;
	int				child_to_wait;
	int				line_valid;
	char			*exit_status;
}	t_element;

typedef struct s_built
{
	t_element		*elements;
	char			**cmd;
	int				neg;
}	t_built;

typedef struct s_var
{
	int				j;
	int				k;
} t_var;

typedef struct s_vars
{
	int				fd;
	char			*line;
}	t_vars;

typedef struct s_env
{
	char			*name;
	char			*key;
	struct s_env	*next;
}	t_env;

//////////////////////ADRI
//UNUSED
typedef struct s_forenvv
{
	t_element		*elements;
	char			*result;
}	t_forenvv;
//

typedef struct s_sqdq
{
	int				sq;
	int				dq;
}	t_sqdq;

typedef struct s_exp
{
	char 			**argv;
	int 			ch;
	int				flag;
	int				code;
}	t_exp;
/////////////////////

typedef struct s_arg
{
	char			**tab;
	t_cmd			**lst;
	int				i;
	int				last_i;
	int				j;
	int				k;
}	t_arg;

extern volatile sig_atomic_t	g_signal;

extern rl_hook_func_t			*rl_event_hook;

// built_in.c
int								is_built_in(char *cmd);
void							ft_built_in(t_element *elements, char **cmd,
									t_gc *gc);
void							built_in_no_child(t_element *elements,
									t_gc *gc);
void							close_other_redir(int i, t_element *elements);

// cd_parta.c
void							ft_change_pwd(t_built *built, t_gc *gc,
									char *buff_old, char *buff);
int								chek_home_and_change_dir(t_built *built,
									t_gc *gc, int ch, char *buff_old);
void							ft_exit_with_success(t_built *built, t_gc *gc,
									int ch);
void							ft_cd(t_built *built, t_gc *gc, int ch);

// cd_partb.c
int								ft_invalid_option(t_built *built, t_gc *gc,
									int ch);
int								ft_no_buff_old(t_built *built, t_gc *gc,
									int ch);
int								ft_home_set(t_built *built, t_gc *gc, int ch,
									char *buff_old);
int								ft_home_not_set(t_built *built, t_gc *gc,
									int ch, char *buff_old);
int								ft_change_direction(t_built *built, t_gc *gc,
									int ch, char *buff_old);

// cd_partc.c
void							cd_error(t_built *built, t_gc *gc);
void							remove_old_env(t_env *head, t_built *built,
									t_gc *gc);
void							change_var_name(t_env *current,
									const char *buff, t_gc *gc);
int								is_home_set(char **env, t_gc *gc);
int								cd_home(t_built *built, t_gc *gc);

// child_creation.c
void							uniq_case(t_element *elements, t_cmd *cmd,
									t_gc *gc);
void							child_process(int i, t_element *elements,
									t_cmd *cmd, t_gc *gc);
void							child_creation(t_element *elements, t_gc *gc);

// child_hedge_cases.c
int								no_child_events(t_element *elements, t_gc *gc,
									t_cmd *current);
void							hedge_child_cases(t_element *elements, t_gc *gc,
									t_cmd *current);

// cmd_arr.c
int	nb_arg(t_arg *arg, t_element *elements, t_gc *gc);
char	**cmd_arr(t_arg *arg, t_element *elements, t_gc *gc);
void	ft_fill_arr(char **arr, t_arg *arg, t_element *elements, t_gc *gc);

// count_lines.c
void		printf_mess_d(char *del, t_element *elements, t_gc *gc);

// create_chain.c
bool		ft_is_redir(char *s);
bool		ft_is_str(char *s);
void		create_chain(t_arg *arg, t_element *elements, t_gc *gc);

// error.c
size_t							ft_strlcat(char *dst, const char *src,
									size_t dstsize);
void							ft_buff_error(char *str, t_element *elements,
									t_gc *gc);
void							ft_write_error(t_element *elements, t_gc *gc);
void							ft_write_mess(t_element *elements, t_gc *gc);

// execution.c
void	path_abs(char **cmd, t_element *elements, t_gc *gc);
void	path_relat(char **cmd, t_element *elements, t_gc *gc);
void							exec_command(t_element *elements, t_gc *gc,
									int i);

// env_manager.c
char							**ft_ltoa(t_env *head, t_gc *gc);
void							ft_env(char **array, char **cmds, t_gc *gc);

//env_manager_utils.c
int								ft_exparser(char *name_key, t_element *elements,
									t_gc *gc);

// fill_arrays.c
void							ft_fill_arrays(t_element *elements, t_gc *gc);

// ft_ambig.c
void							ft_ambig(char *result_k, int *k,
									t_element *elements, t_gc *gc);

// ft_concat.c
char							*ft_concat(char *result_i, int k, int sq,
									int dq);

// ft_echo.c
void							ft_echo(char **cmd, t_gc *gc);

// ft_exit.c
int								ft_exit(t_built *built, t_gc *gc, int ch);
int								ft_isdigitexit(char *num);

//ft_exiterror.c
void							ft_exitfail(t_built *built, t_gc *gc);
int								ft_error_many(t_built *built, t_gc *gc, int ch);
int								ft_checkexit(t_built *built, t_gc *gc, int ch);

// ft_export_tools.c
char							*ft_cut(const char *src, char delim, int is_end,
									t_gc *gc);
void							ft_printexport(const t_env *head,
									t_element *elements, t_gc *gc);
void							ft_adder(t_env **head, char *str, t_gc *gc);
int								ft_dr(t_exp *xx, t_env *head, t_element *element, t_gc *gc);
int								ft_opp(t_exp *xx, t_env *head, t_element *element, t_gc *gc);

// ft_export.c
char							**ft_export(t_element *element, char **argv,
									t_gc *gc, int ch);

// ft_export_utils.c
t_env							*ft_envnode(char *name, char *key, t_gc *gc);
t_env							*ft_addenvnode(t_env *head, char *name,
									char *key, t_gc *gc);
void							ft_swapnodes(t_env *node1, t_env *node2);
void							ft_bbsort(t_env *head);

// ft_getenvv.c
char	*ft_getenvv(char *result, int *k, char *tmp, t_element *elements, t_gc *gc);
// char	*ft_getenvv(t_forenvv *ev, char *tmp, int *k, t_gc *gc);
char							*ft_itoa(int nb, t_gc *gc);
void	ft_spacequotes(char	*result_k, t_element *elements, t_gc *gc);

// ft_ifexpand.c
char							*ft_ifexpand(char *result, t_sqdq *q,
									t_element *elements, t_gc *gc);

// ft_split.c
char							**ft_split(char const *str, int sq, int dq,
									t_gc *gc);

// ft_split_utils.c
size_t							ft_count(const char *str, const char sep);
char							*ft_eachword(const char *str, int fidx, int end,
									t_gc *gc);

// ft_splitboloss.c
char	**ft_splitboloss(char const *str, char sep, t_gc *gc);

// ft_tokenize.c
char							*ft_tokenize(char *av2, t_gc *gc,
									t_element *elements);

// ft_unset.c
void							ft_unset(t_element *element, char **argv,
									t_gc *gc, int ch);
//ft_unset_utils.c
int		ft_initunset(t_env **ptr_head, char **env, char **argv, t_gc *gc);
void	ft_freeun(t_env **current, t_env **previous, t_env **head, t_gc *gc);

// here_exp.c
char							*ft_hereifexpand(char *result,
									t_element *elements, t_gc *gc);
char	*ft_hedgifexp(char *result, t_element *elements, t_gc *gc);


// here_utils.c
void	no_here_out(t_cmd *node, t_file *redir, t_element *elements, t_gc *gc);
int		ft_open_heredoc_error(char *del, t_element *elements, t_gc *gc);

// heredoc.c
int								ft_open_heredoc(char *del, t_element *elements,
									t_gc *gc);


// init.c
void							ft_welcome(void);
void							ft_cpy_env(t_element *elements, char **env,
									t_gc *gc, char **argv);
t_element						*ft_init_struct(t_gc *gc);

// lexing.c
void							hedge_case_1(char **tab, t_element *elements,
									t_gc *gc);
int								ft_istok_2(char *av2);
void							lexing(char **tab, t_cmd **lst,
									t_element *elements, t_gc *gc);

// libft_a.c
size_t							ft_strlen(const char *str);
void							*ft_calloc(size_t n, size_t sizof);
int								ft_strcmp(char *s1, char *s2);
int								ft_strncmp(const char *str1, const char *str2,
									size_t n);
int								ft_isalnum(int c);

// libft_abis.c
char							*ft_strdup_(char const *str, t_gc *gc);
char							*ft_strncpy(char *dest, const char *src,
									size_t n);
char							*ft_strcat(char *dst, const char *src);
int								ft_isalpha(int c);
char							*ft_strjoin_(char *str1, char *str2, t_gc *gc);

// libft_e.c
int								ft_arr_size(char **tab);
char							*ft_strdup(const char *s1, t_gc *gc);
char							*ft_substr(char const *s, unsigned int start,
									size_t len, t_gc *gc);
void							ft_putstr_fd(char *s, int fd);
int								ft_atoi(const char *str);

// libft_ebis.c
char		*ft_strjoin(char *s1, char *s2, t_gc *gc);
void	write_all_err(char *s1, char *s2, t_element *elements, t_gc *gc);
int is_directory(char *path);
void	exit_status(int status, t_element *elements, t_gc *gc);
int		ft_bst(char *name);

// parsing_utils.c
void							ft_deldollar(char *input);
int								ft_istok_(char *av2);

// parsing.c
void							ft_ft(t_element *elements, t_gc *gc);

// pipe_closing.c
void							first_cmd_with_valid_infile(t_element *elements,
									t_cmd *cmd, t_gc *gc);
void							last_cmd_with_valid_outfile(t_element *elements,
									t_cmd *cmd, t_gc *gc);
void							dup_close_read_pipe(int k,
									t_element *elements, t_cmd *cmd, t_gc *gc);
void							dup_close_write_pipe(int k,
									t_element *elements, t_cmd *cmd, t_gc *gc);
void							all_cases(int i, t_element *elements,
									t_cmd *cmd, t_gc *gc);

// pipe_handle.c
void							pipe_creation(t_element *elements, t_gc *gc);
void							close_pipes(t_element *elements);
void							part_close(t_element *elements, int k);
void							wait_for_children(t_element *elements,
									t_gc *gc);
void							free_std(void);

// pwd.c
void							ft_pwd(char **cmd, t_element *elements,
									t_gc *gc);
int								is_invalid_option(char *str);

// signal_bis.c
void	reset_signal_status(void);
void	handle_sigpipe(int sig);

// signal.c
void	ft_handle_signal(int process);

// split_paths.c
void							ft_handle_path(t_element *elements, t_gc *gc);

// str_manager.c
char							*ft_insert(char *result, int k, char c);
char							*ft_erase(char *result, int tmp_k);
void							ft_erase_substr(char *result, int *k,
									char *tmp);
void							ft_modifquote_(char const *str, int *sq,
									int *dq, int *i);

// redir_chain.c
int		ft_bst(char *name);
t_file	*create_redir(t_arg *arg, t_element *elements, t_gc *gc);
char		*ft_filter(char *name, t_element *elements, t_gc *gc);

// redir_open.c
void							ft_fd_open(t_cmd *node, t_element *elements,
									t_gc *gc, int flag);
void							handle_redir(t_cmd **lst, t_element *elements,
									t_gc *gc);
void							ft_handle_in(t_cmd *node, t_file *redir,
									t_element *elements, t_gc *gc);
void							ft_handle_out(t_cmd *node, t_file *redir,
									t_element *elements, t_gc *gc);
void							ft_error_out(char *name, t_element *elements,
									t_gc *gc);
void							ft_handle_no_here_out(t_cmd *node,
									t_file *redir, t_element *elements,
									t_gc *gc);

// pour test
void							print_cmd_list(t_cmd *cmd_list);
void							check_fds(const char *context);

#endif
