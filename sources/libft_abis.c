#include "../minishell.h"

char	*ft_strdup_(char const *str)
{
	char	*dest;
	int	i;

	i = -1;
	dest = malloc(ft_strlen(str) + 1);
	if (dest == NULL)
		return (NULL);//gc_cleaner
	while (str[++i])
		dest[i] = str[i];
	dest[i] = '\0';
	return (dest);
}

char *ft_strncpy(char *dest, const char *src, size_t n)
{
    size_t i;

    i = -1;
    while (src[++i] && i < n)
        dest[i] = src[i];
    while (i++ < n)
        dest[i] = '\0';
    return dest;
}