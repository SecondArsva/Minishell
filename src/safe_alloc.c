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
