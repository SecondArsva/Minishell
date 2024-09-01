#include "../includes/minishell.h"

void	env_check_var_len(t_data *data, char *finded_var)
{
	t_env	*env;

	env = data->env;
	while (env)
	{
		if (!ft_strncmp(finded_var, env->var_name,
				ft_strlen(env->var_name) + 1))
		{
			data->exp_len += ft_strlen(env->var_value);
			return ;
		}
		env = env->next;
	}
}

void	env_len(t_data *data, char *val)
{
	char	*finded_var;
	int		entry_value;

	finded_var = NULL;
	data->i++;
	entry_value = data->i;
	while (val[data->i] && is_validenvchar(val[data->i]))
		data->i++;
	finded_var = ft_substr(val, entry_value, (data->i - entry_value));
	env_check_var_len(data, finded_var);
	free(finded_var);
}

void	exp_exit_status_len(t_data *data)
{
	char	*aux_str;

	aux_str = ft_itoa(data->exit_status);
	data->i += 2;
	data->exp_len += ft_strlen(aux_str);
	free(aux_str);
}

void	format_len(t_data *data, char *val)
{
	if ((!val[data->i + 1] || val[data->i + 1] == ' ' || data->in_s_quot)
		|| (val[data->i + 1] == '"' && data->in_d_quot))
	{
		data->i++;
		data->exp_len++;
	}
	else if ((val[data->i + 1] == '\"' || val[data->i + 1] == '\'')
		&& !data->quoted)
		data->i++;
	else if (val[data->i + 1] == '$' && !data->in_s_quot)
		data->i += 2;
	else if (is_validenvchar(val[data->i + 1]))
		env_len(data, val);
	else if (val[data->i + 1] == '?')
		exp_exit_status_len(data);
	else
	{
		data->i++;
		data->exp_len++;
	}
}

int	exp_token_len(t_data *data, char *val)
{
	data->i = 0;
	data->exp_len = 0;
	while (val[data->i])
	{
		exp_into_quotes(data, val);
		if (val[data->i] == '$')
			format_len(data, val);
		else
		{
			data->i++;
			data->exp_len++;
		}
	}
	data->exp_len++;
	printf("data->exp_len: %i\n", data->exp_len);
	return (data->exp_len);
}
