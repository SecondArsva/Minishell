#include "../includes/minishell.h"

void	env_update_tail(t_env *head, t_env *new_tail)
{
	t_env	*tmp;

	tmp = head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_tail;
}

char *env_grab_value(t_data *data, char **env)
{
	char	*value;
	int		start;

	value = NULL;
	start = 0;
	if (env[data->i][data->j] && env[data->i][data->j] == '=')
		data->j++;
	else
		return (NULL);
	if (env[data->i][data->j] && env[data->i][data->j] == '"')
		data->j++;
	start = data->j;
	while (env[data->i][data->j]
	&& (env[data->i][data->j] != '=' && env[data->i][data->j + 1]))
		data->j++;
	value = ft_substr(env[data->i], start, (data->j+1) - start);
	return (value);
}

char *env_grab_name(t_data *data, char **env)
{
	char *name;

	name = NULL;
	while (env[data->i][data->j] && env[data->i][data->j] != '=')
		data->j++;
	name = ft_substr(env[data->i], 0, data->j);
	return (name);
}

// En un futuro puedo añadir otros dos parámetros char * a la función con los
// que poder añadir directamente el nombre de la variable y su valor cuando
// se vaya a hacer un export.
// void	env_new_node(t_data *data, char **env, char *name, char *value)
// y en caso de que sea null, que se pille del env tal y como está.
void	env_new_node(t_data *data, char **env)
{
	t_env	*new_node;

	data->j = 0;
	new_node = NULL;
	new_node = (t_env *) safe_malloc(sizeof(t_env));
	new_node->var_name = env_grab_name(data, env);
	new_node->var_value = env_grab_value(data, env);
	new_node->next = NULL;
	if (!data->env)
		data->env = new_node;
	else
		env_update_tail(data->env, new_node);
}

void	init_env(t_data *data, char **env)
{
	data->i = 0;

	data->env = NULL;
	if (!env)
		data->env = NULL;
	else
	{
		while (env[data->i])
		{
			env_new_node(data, env);
			data->i++;
		}
	}
}