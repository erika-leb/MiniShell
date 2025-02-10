/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_e.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:43 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/10 11:48:06 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

int	ft_arr_size(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *s1, t_gc *gc)
{
	char	*str;
	int		i;

	i = 0;
	str = (char *) gc_malloc((ft_strlen(s1) + 1) * sizeof(char), gc);
	if (str == 0)
		return (NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_substr(char const *s, unsigned int start, size_t len, t_gc *gc)
{
	char	*str;
	size_t	i;

	i = 0;
	if (s == NULL)
		return (NULL);
	if (start > ft_strlen(s))
		return (ft_strdup("", gc));
	if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	str = gc_malloc(sizeof(char) * (len + 1), gc);
	if (str == 0)
		return (NULL);
	while (i < len && s[start + i])
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	//perror("on entre");
	i = 0;
	if (s == NULL)
		return ;
	while (s[i])
	{
		//printf("s[%d] = %c\n", i, s[i]);
		write(fd, &s[i], 1);
		i++;

	}
	//perror("on sort");
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	r;

	i = 0;
	sign = 1;
	r = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
			i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
			r = r * 10 + str[i] - '0';
			i++;
	}
	return (r * sign);
}

// char	*get_next_line(int fd, t_gc *gc)
// {
// 	static char	buffer[BUFFER_SIZE];
// 	static int	buff_read;
// 	static int	buff_pos;
// 	int			i = 0;
// 	char		line[70000];

// 	if (fd < 0 || BUFFER_SIZE <= 0)
// 		return (NULL);
// 	while (1)
// 	{
// 		if (buff_pos >= buff_read)
// 		{
// 			buff_read = read(fd, buffer, BUFFER_SIZE);
// 			if (buff_read <= 0)
// 				break;
// 			buff_pos = 0;
// 		}
// 		line[i++] = buffer[buff_pos++];
// 		if (line[i - 1] == '\n')
// 			break;
// 	}
// 	line[i] = '\0';
// 	if (i == 0)
// 		return (NULL);
// 	return (ft_strdup(line, gc));
// }

