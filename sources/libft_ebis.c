/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_ebis.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 17:13:53 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/13 14:33:57 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strjoin(char *s1, char *s2, t_gc *gc)
{
	char	*str;
	int		i;
	int		size1;
	int		size2;
	int		j;

	size1 = ft_strlen(s1);
	size2 = ft_strlen(s2);
	str = gc_malloc(sizeof(char) * (size1 + size2 + 1), gc);
	i = 0;
	j = 0;
	if (str == 0)
		return (NULL);
	while (i < size1)
	{
		str[i] = s1[i];
		i++;
	}
	while (i < size1 + size2)
		str[i++] = s2[j++];
	str[i] = '\0';
	return (str);
}

void	write_all_err_mess(char *s1, char *s2, t_element *elements, t_gc *gc)
{
	ft_buff_error("minishell: ", elements, gc);
	ft_buff_error(s1, elements, gc);
	ft_buff_error(s2, elements, gc);
	ft_write_error(elements, gc);
}

int is_directory(char *path)
{
	struct stat sb;

	if (stat(path, &sb) == 0) // Récupère les infos du fichier
	{
		return S_ISDIR(sb.st_mode); // Vérifie si c'est un dossier
	}
	return 0; // Erreur ou ce n'est pas un dossier
}
