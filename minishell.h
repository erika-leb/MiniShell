/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:29:10 by aisidore          #+#    #+#             */
/*   Updated: 2024/11/16 14:29:12 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>//printf
# include <stdlib.h>//free, ...
# include <stdint.h>//SIZE_MAX

//ft_split.c
char	**ft_split(char const *str, char sep, int sq, int dq);

//ft_split_utils.c
size_t	ft_count(const char *str, const char sep);
char	*ft_eachword(const char *str, int fidx, int end);
void	*ft_freesplit(char **tab, size_t n);

//libft.c
void	*ft_calloc(size_t n, size_t sizof);
size_t	ft_strlen(const char *str);

#endif
