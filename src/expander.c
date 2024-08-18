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
*/ 
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

void	manage_expansion(t_data *data, t_token *node)
{
	char	*exp_str;

	exp_str = safe_calloc(exp_token_len(data, node->value), sizeof(char *));
	(void)exp_str;
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