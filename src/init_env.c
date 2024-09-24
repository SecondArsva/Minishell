#include "../includes/minishell.h"

void	env_update_tail(t_env *head, t_env *new_tail)
{
	t_env	*tmp;

	tmp = head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_tail;
}

char*	env_quoting_value(t_data *data, char **env, char *value, int start)
{
	if (env[data->i][start] == '"')
	{
		value = safe_strdup("'");
		value = safe_strjoin(value, safe_substr(env[data->i], start, (data->j + 1) - start));
		value = safe_strjoin(value, "'");
	}
	else
	{
		value = safe_strdup("\"");
		value = safe_strjoin(value, safe_substr(env[data->i], start, (data->j + 1) - start));
		value = safe_strjoin(value, "\"");
	}
	return (value);
}

char	*env_grab_value(t_data *data, char **env)
{
	char	*value;
	int		start;

	value = NULL;
	start = 0;
	if (env[data->i][data->j] && env[data->i][data->j] == '=')
		data->j++;
	else
		return (NULL);
	start = data->j;
	while (env[data->i][data->j]
	&& (env[data->i][data->j] != '=' && env[data->i][data->j + 1]))
		data->j++;
	value = env_quoting_value(data, env, value, start);
	return (value);
}

char	*env_grab_name(t_data *data, char **env)
{
	char	*name;

	name = NULL;
	while (env[data->i][data->j] && env[data->i][data->j] != '=')
		data->j++;
	name = safe_substr(env[data->i], 0, data->j);
	return (name);
}

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

void	increase_shlvl(t_env *env)
{
	t_env	*tmp;
	int		i;
	char	*aux_one;
	char	*aux_two;

	tmp = env;
	while (tmp)
	{
		if (!ft_strncmp("SHLVL", tmp->var_name, ft_strlen("SHLVL") + 1))
			break ;
		tmp = tmp->next;
	}
	aux_one = safe_strtrim(tmp->var_value, "\"");
	free(tmp->var_value);
	i = ft_atoi(aux_one);
	free(aux_one);
	aux_one = safe_itoa(i + 1);
	aux_two = safe_strjoin("\"", aux_one);
	tmp->var_value = safe_strjoin(aux_two, "\"");
	free(aux_one);
	free(aux_two);
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
		increase_shlvl(data->env);
	}
}
