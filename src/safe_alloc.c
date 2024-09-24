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
char	*safe_substr(char const *s, unsigned int start, size_t len)
{
	size_t	j;
	char	*new_ptr;

	j = 0;
	if (!s)
		return (0);
	if (start < ft_strlen(s))
	{
		if (len > ft_strlen(s) - start)
			len = ft_strlen(s) - start;
		new_ptr = safe_calloc(len + 1, sizeof(char));
		if (!new_ptr)
			return (0);
		while (s[start] != '\0' && j < len)
		{
			new_ptr[j] = s[start + j];
			j++;
		}
		new_ptr[j] = '\0';
		return (new_ptr);
	}
	new_ptr = safe_calloc(1, sizeof(char));
	if (!new_ptr)
		return (0);
	return (new_ptr);
}

static size_t	ft_digitcounter(long int n)
{
	size_t	digits;

	if (n <= 0)
		digits = 1;
	else
	{
		digits = 0;
		n = n * 1;
	}
	while (n != 0)
	{
		n = n / 10;
		digits++;
	}
	return (digits);
}

char	*safe_itoa(int n)
{
	size_t		len;
	size_t		isneg;
	char		*new_ptr;

	if (n == -2147483648)
		return (safe_strdup("-2147483648"));
	len = ft_digitcounter(n);
	isneg = 0;
	new_ptr = safe_malloc((len + 1) * sizeof(char));
	if (!new_ptr)
		return (0);
	if (n < 0)
	{
		new_ptr[0] = '-';
		n = n * -1;
		isneg = 1;
	}
	new_ptr[len] = 0;
	while (len > (0 + isneg))
	{
		new_ptr[len - 1] = n % 10 + '0';
		n = n / 10;
		len--;
	}
	return (new_ptr);
}

char	*safe_strtrim(char const *s1, char const *set)
{
	size_t		i;
	size_t		j;
	char		*new_ptr;

	i = 0;
	j = ft_strlen(s1);
	if (!s1)
		return (0);
	if (!set)
		return (safe_strdup(s1));
	while (s1[i] && ft_strchr(set, s1[i]))
		i++;
	while (j > 0 && ft_strchr(set, s1[j]))
		j--;
	new_ptr = safe_substr(s1, i, (j + 1) - i);
	return (new_ptr);
}

