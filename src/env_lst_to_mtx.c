#include "../includes/minishell.h"

/*
	[x] TODO conversión de t_env a char ** para pasar el env con sus
			 datos actualizados a través de execve.

		- Recuerda "trimear" las comillas del primer tipo encontrado.
		- No hay que liberar lo encontrado en la env_lst ya que hay que
		  recordar la que conversión se hace para poder trabajar en un
		  proceso hijo, por lo que todo lo que haya en el padre ha de
		  mantenerse.
*/
char	**fill_matrix(t_env *env_lst, int len)
{
	char	**matrix;
	int		i;
	char	*fist_join;
	char	*last_join;
	char	*var_value_trimmed;

	i = 0;
	matrix = safe_calloc(len + 1, sizeof(char **));
	while (i < len)
	{
		var_value_trimmed = env_lst->var_value;
		if (env_lst->var_value[0] == '"')
			var_value_trimmed = ft_strtrim(env_lst->var_value, "\"");
		else if (env_lst->var_value[0] == '\'')
			var_value_trimmed = ft_strtrim(env_lst->var_value, "\'");
		fist_join = safe_strjoin(env_lst->var_name, "=");
		last_join = safe_strjoin(fist_join, var_value_trimmed);
		free(fist_join);
		matrix[i] = last_join;
		env_lst = env_lst->next;
		i++;
	}
	matrix[i] = NULL;
	return (matrix);
}

char	**env_lst_to_mtx(t_env *env_lst)
{
	t_env	*tmp;
	int		len;

	tmp = env_lst;
	len = 0;
	while (tmp)
	{
		tmp = tmp->next;
		len++;
	}
	tmp = env_lst;
	return (fill_matrix(tmp, len));
}
