#include "../minishell.h"


int	ft_isalpha(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (1);
	if (c >= 'a' && c <= 'z')
		return (1);
	return (0);
}

char	*ft_strdup_(char const *str, t_gc *gc)
{
	char	*dest;
	int	i;

	i = -1;
	dest = gc_malloc(ft_strlen(str) + 1, gc);
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

// char	*ft_strjoin_(char *str1, char *str2, t_gc *gc)
// {
// 	size_t	len_str1;
// 	size_t	len_str2;
// 	size_t	i;
// 	size_t	j;
// 	char	*str;

// 	len_str1 = ft_strlen(str1);
// 	len_str2 = ft_strlen(str2);
// 	i = -1;
// 	j = -1;
// 	str = gc_calloc(len_str1 + len_str2 + 1, sizeof(char), gc);
// 	while (++i < len_str1)
// 		str[i] = str1[i];
// 	while (++j < len_str2)
// 		str[i + j] = str2[j];
// 	str[i + j] = '\0';
// 	//str1 estfree car heap. Dans nouvelle version il se peut que str2 soit aussi de la heap
// 	//gc_remove(gc, str1);
// 	return (free(str1), str);
// }
