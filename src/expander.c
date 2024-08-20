#include "../includes/minishell.h"

// función para ver si el caracter es válido dentro del set de caracteres aceptados por una variable de entorno. Alfanuméricos (mayúsculas y minúsculas) y el guión bajo.
int is_validenvchar(char c)
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

/*
* En esta función capturaré todos los caracteres que vayan detrás del dólar
* hasta encontrarme con uno no válido dentro del nombre de las variables
* de entorno, lo almacenaré y lo buscaré en el enviroment para expandir a
* valor en caso de existir o expandirlo a nada en caso de que no exista.
void	grab_var_name(t_data *data, char *value, char *env)
{
	char	var_to_expand[100];
	int		i;

	i = 0;
	while (value[data->i])
	{
		if (is_validenvchar(value[data->i]))
		{
			var_to_expand[i] = value[data->i];
			data->i++;
			i++;
		}
		else
		{
			var_to_expand[i];
			break ;
		}
	}
}
*/

/*
* echo hola$ hola$\0
* hola$"" hola$''
* '$U...SER'

* $? Exit Status
* $VAR Valid Variable
* $"" en este caso el dolar ha de desaparecer
* $NO_EXIST_VAR Invalid Variable expand to nothing
void	format_type(t_data *data, char *value, char *exp_str)
{
	if (!value[data->i + 1] || value[data->i + 1] == ' ' || data->in_s_quot || (value[data->i + 1] == '"' && data->in_d_quot)) 
	{
		exp_str[data->j] = '$';
		data->i++;
		data->j++;
	}
	else if ((value[data->i + 1] == '\"' || value[data->i + 1] == '\'') && !data->quoted)
		data->i++;
	else if (value[data->i + 1] == '$' && !data->in_s_quot)
		data->i = data->i + 2;
	//else if (is_validvarchar(value[data->i + 1]))
	//{
	//	grab_var_name(data, value, exp_str);
	//}
}
*/ 

/*
 * Copia el carácter actual en el que se encuentra el iterador de value.
 */
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
		if (!ft_strncmp(finded_var, env->var_name, ft_strlen(finded_var)))
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

void	format_exp(t_data *data, char *val)
{
	if ((!val[data->i + 1] || val[data->i + 1] == ' ' || data->in_s_quot)
	|| (val[data->i + 1] == '"' && data->in_d_quot))
		exp_cpy_char(data, val);	// OK
	else if ((val[data->i + 1] == '\"' || val[data->i + 1] == '\'') && !data->quoted)
		data->i++;					// OK
	else if (val[data->i + 1] == '$' && !data->in_s_quot)
		data->i += 2;				// OK
	else if (is_validenvchar(val[data->i + 1]))
		env_exp(data, val);			// OK?
	//else if (val[data->i + 1] == '?')
	//	exp_exit_status_len(data);
	else
		exp_cpy_char(data, val);	// OK
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
		else // copy char
			exp_cpy_char(data, val);
	}
	data->exp_str[data->j] = '\0';
}

void	manage_expansion(t_data *data, t_token *node)
{
	data->exp_str = NULL;
	data->exp_str = safe_calloc(exp_token_len(data, node->value), sizeof(char *));
	// fill exp_str with the new value expanded
	exp_val(data, node->value);
	// free node->value
	free(node->value);
	// upload node->value content to exp_str
	node->value = ft_strdup(data->exp_str);
	// free data->exp_str
	free(data->exp_str);
	/*
	while (node->value[data->i])
	{
		printf("%c %i\n", node->value[data->i], data->i);
		exp_into_quotes(data, node->value);
		if (node->value[data->i] == '$')
			format_type(data, node->value, expanded_str);
		else
		{
			expanded_str[data->j] = node->value[data->i];
			data->i++;
			data->j++;
		}
		if (!node->value[data->i])
			break ;
	}
	expanded_str[data->j] = '\0';
	printf("expanded_string: %s\n", expanded_str);
	*/
}

/*
	Revisa la lista de los tokens y procesa el valor de
	los nodos en caso de encontrar un dólar.
*/
void	expander(t_data *data)
{
	t_token *tmp_lst;

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