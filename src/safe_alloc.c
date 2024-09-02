#include "../includes/minishell.h"

void	*safe_malloc(size_t bytes)
{
	void	*reserve;

	reserve = malloc(bytes);
	if (!reserve)
		exit(EXIT_FAILURE);
	return (reserve);
}

void	*safe_calloc(size_t count, size_t size)
{
	void	*ptr;
	size_t	i;

	i = 0;
	ptr = (void *) safe_malloc(count * size);
	while (i < (count * size))
	{
		((char *) ptr)[i] = '\0';
		i++;
	}
	return (ptr);
}

char	**safe_matrixdup(char **original)
{
	char	**duplicate;
	size_t	i;

	i = 0;
	while (original[i])
		i++;
	duplicate = safe_calloc(i + 1, sizeof(char **));
	i = 0;
	while (original[i])
	{
		duplicate[i] = ft_substr(original[i], 0, -1);
		if (!duplicate[i])
			exit(EXIT_FAILURE);
		i++;
	}
	return (duplicate);
}

char	*safe_strjoin(char const *s1, char const *s2)
{
	char	*new_ptr;
	size_t	s1_len;
	size_t	s2_len;

	if (!s1 || !s2)
		return (0);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	new_ptr = safe_calloc(s1_len + s2_len + 1, sizeof(char));
	if (!new_ptr)
		return (0);
	ft_strlcpy(new_ptr, s1, s1_len + 1);
	ft_strlcat(new_ptr + s1_len, s2, s2_len + 1);
	return (new_ptr);
}

char	*safe_strdup(const char *s1)
{
	int		i;
	char	*new_ptr;

	i = 0;
	new_ptr = safe_malloc(ft_strlen(s1) + 1);
	if (!new_ptr)
		return (0);
	while (s1[i] != '\0')
	{
		new_ptr[i] = s1[i];
		i++;
	}
	new_ptr[i] = '\0';
	return (new_ptr);
}
