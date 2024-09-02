#include "../includes/minishell.h"

void	quo_new_val(t_data *data, char *val)
{
	char	actual_quote;

	actual_quote = '*';
	data->i = 0;
	data->j = 0;
	while (val[data->i])
	{
		if ((val[data->i] == '"' || val[data->i] == '\'')
			&& !ft_strchr("\"\'", actual_quote))
		{
			actual_quote = val[data->i];
			data->i++;
		}
		else if (ft_strchr("\"\'", actual_quote)
			&& val[data->i] == actual_quote)
		{
			actual_quote = '*';
			data->i++;
		}
		else
			exp_cpy_char(data, val);
	}
	data->exp_str[data->j] = '\0';
}

void	quo_new_len(t_data *data, char *val)
{
	char	act_quote;

	act_quote = '*';
	data->exp_len = 0;
	while (val[data->i])
	{
		if ((val[data->i] == '"' || val[data->i] == '\'')
			&& !ft_strchr("\"\'", act_quote))
		{
			act_quote = val[data->i];
			data->i++;
		}
		else if (ft_strchr("\"\'", act_quote) && val[data->i] == act_quote)
		{
			act_quote = '*';
			data->i++;
		}
		else
		{
			data->i++;
			data->exp_len++;
		}
	}
	data->exp_len++;
}

void	manage_quotes(t_data *data, t_token *node)
{
	data->exp_str = NULL;
	quo_new_len(data, node->value);
	data->exp_str = safe_calloc(data->exp_len, sizeof(char *));
	quo_new_val(data, node->value);
	free(node->value);
	node->value = safe_strdup(data->exp_str);
	free(data->exp_str);
}

void	quoter(t_data *data)
{
	t_token	*tmp_lst;

	tmp_lst = data->tokens;
	while (tmp_lst)
	{
		data->i = 0;
		data->j = 0;
		if (tmp_lst->value && (ft_strchr(tmp_lst->value, '"')
				|| ft_strchr(tmp_lst->value, '\'')))
			manage_quotes(data, tmp_lst);
		tmp_lst = tmp_lst->next;
	}
}
