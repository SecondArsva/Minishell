#include "../includes/minishell.h"

int	is_validenvchar(char c)
{
	if (c == '_' || (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
		return (1);
	else
		return (0);
}

void	exp_into_quotes(t_data *data, char *value)
{
	if (value[data->i] == '\"' && !data->in_d_quot && !data->in_s_quot)
		data->in_d_quot = true;
	else if (value[data->i] == '\"' && data->in_d_quot)
		data->in_d_quot = false;
	else if (value[data->i] == '\'' && !data->in_s_quot && !data->in_d_quot)
		data->in_s_quot = true;
	else if (value[data->i] == '\'' && data->in_s_quot)
		data->in_s_quot = false;
	if (data->in_s_quot == true || data->in_d_quot == true)
		data->quoted = true;
	else
		data->quoted = false;
}

void	exp_cpy_char(t_data *data, char *val)
{
	data->exp_str[data->j] = val[data->i];
	data->i++;
	data->j++;
}

void	env_check_var_exp(t_data *data, char *finded_var)
{
	t_env	*env;
	int		i;

	env = data->env;
	i = 0;
	while (env)
	{
		if (!ft_strncmp(finded_var, env->var_name,
				ft_strlen(env->var_name) + 1))
		{
			while (env->var_value[i])
			{
				data->exp_str[data->j] = env->var_value[i];
				data->j++;
				i++;
			}
			return ;
		}
		env = env->next;
	}
}

void	env_exp(t_data *data, char *val)
{
	char	*finded_var;
	int		entry_value;

	finded_var = NULL;
	data->i++;
	entry_value = data->i;
	while (val[data->i] && is_validenvchar(val[data->i]))
		data->i++;
	finded_var = ft_substr(val, entry_value, (data->i - entry_value));
	env_check_var_exp(data, finded_var);
	free(finded_var);
}

void	exp_exit_status(t_data *data)
{
	char	*aux_str;
	int		i;

	i = 0;
	aux_str = ft_itoa(data->exit_status);
	data->i += 2;
	while (aux_str[i])
	{
		data->exp_str[data->j] = aux_str[i];
		data->j++;
		i++;
	}
	free(aux_str);
}

void	format_exp(t_data *data, char *val)
{
	if ((!val[data->i + 1] || val[data->i + 1] == ' ' || data->in_s_quot)
		|| (val[data->i + 1] == '"' && data->in_d_quot))
		exp_cpy_char(data, val);
	else if ((val[data->i + 1] == '\"' || val[data->i + 1] == '\'')
		&& !data->quoted)
		data->i++;
	else if (val[data->i + 1] == '$' && !data->in_s_quot)
		data->i += 2;
	else if (is_validenvchar(val[data->i + 1]))
		env_exp(data, val);
	else if (val[data->i + 1] == '?')
		exp_exit_status(data);
	else
		exp_cpy_char(data, val);
}

void	exp_val(t_data *data, char *val)
{
	data->i = 0;
	data->j = 0;

	while (val[data->i])
	{
		exp_into_quotes(data, val);
		if (val[data->i] == '$')
			format_exp(data, val);
		else
			exp_cpy_char(data, val);
	}
	data->exp_str[data->j] = '\0';
}

void	manage_expansion(t_data *data, t_token *node)
{
	data->exp_str = NULL;
	data->exp_str = safe_calloc(exp_token_len(data, node->value),
			sizeof(char *));
	exp_val(data, node->value);
	free(node->value);
	node->value = ft_strdup(data->exp_str);
	free(data->exp_str);
}

void	expander(t_data *data)
{
	t_token	*tmp_lst;

	tmp_lst = data->tokens;
	while (tmp_lst)
	{
		data->i = 0;
		data->j = 0;
		data->in_d_quot = 0;
		data->in_s_quot = 0;
		data->quoted = 0;
		if (tmp_lst->value && ft_strchr(tmp_lst->value, '$'))
			manage_expansion(data, tmp_lst);
		tmp_lst = tmp_lst->next;
	}
}
