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

char *ft_strcat(char *dst, const char *src)
{
    size_t dstlen;
    size_t i;

    dstlen = ft_strlen(dst);
    i = 0;
    while (src[i])
    {
        dst[dstlen + i] = src[i];
        i++;
    }
    dst[dstlen + i] = '\0';
    return (dst);
}