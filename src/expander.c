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

void	env_check_var_len(t_data *data, char *finded_var)
{
	t_env	*env;

	env = data->env;
	while (env)
	{
		printf("Hola\n");
		if (!ft_strncmp(finded_var, env->var_name, ft_strlen(finded_var)))
		{
			data->exp_len += ft_strlen(env->var_value);
			printf("%s - %s\n", env->var_name, env->var_value);
			printf("var len: %li\n", ft_strlen(env->var_value));
			return ;
		}
		env = env->next;
	}
}

void	env_len(t_data *data, char *val)
{
	printf("entra2\n");
	char	*finded_var;
	int		entry_value;
	
	finded_var = NULL;
	data->i++;
	entry_value = data->i;
	while (val[data->i] && is_validenvchar(val[data->i]))
		data->i++;
	finded_var = ft_substr(val, entry_value, (data->i - entry_value));
	printf("---- %s\n", finded_var);
	sleep(1);
	env_check_var_len(data, finded_var);
}

void	exp_exit_status_len(t_data *data)
{
	char	*aux_str;

	aux_str = ft_itoa(data->exit_status);
	data->i += 2;
	data->exp_len += ft_strlen(aux_str);
	free(aux_str);
}

/* Función en la que se determinará que hacer según lo que haya después del
 * dólar, ya que no siempre que nos encontremos uno se gestionará de la
 * misma forma.
 * Al momento de entrar en esta función, el iterador sigue en la posición del
 * dólar a procesar, por lo que tendré que tener en cuenta los carácteres que
 * le sigan.
 * 
 * Casos:
 * 	- Se aumenta en +1 ya que el dólar se deberá pintar en todos los casos:
 * 		token$
 * 		token$ token2
 * 		'token$'
 * 		"token$"
 * 		'token$"'
 * 		"token$'"
 * 
 *  - Si encuentro dos dólares seguidos y siempre que no estén entre comillas
 *    simples se aumenta en +2 ya que ambos dólares de deben omitir al no
 *    poder acceder al PID:
 * 		$$
 * 		token$$
 * 		"$$token"
 * 
 *  - Si encuentro carácteres que podrían usarse a la hora de nombrar una
 *    variable de entorno (letras en mayúsculas y minúsculas, números y
 *    barras bajas) encapsulo tantos carácteres válidos como encuentre y
 *    verifico en el env si la expansión se hará al valor de una variable
 *    de entorno existente o si se expande a nada al no exsistir lo que sea
 *    que encuentre como supuesto nombre de variable a expandir, por lo que
 *    se aumentará el iterador o no según lo que se encuentre.
 * 
 *  - Me falta añadir el caso de la expansión al "Exit Status",
 *    el dólar interrogación, vamos... "$?"
 * 
 *  - ¿Debería contar todo lo demás como un +1 y printeo del dólar? Si.
 *    echo $- = himBHs
 *    echo $% = $%
 */
void	format_len(t_data *data, char *val)
{
	printf("entra1\n");
	if ((!val[data->i + 1] || val[data->i + 1] == ' ' || data->in_s_quot)
	|| (val[data->i + 1] == '"' && data->in_d_quot)
	|| ((val[data->i + 1] == '\"' || val[data->i + 1] == '\'') && !data->quoted))
	{
		printf("1\n");
		data->i++;
		data->exp_len++;
	}
	else if (val[data->i + 1] == '$' && !data->in_s_quot)
	{
		printf("2\n");
		data->i += 2;
	}
	else if (is_validenvchar(val[data->i + 1]))
	{
		printf("3\n");
		env_len(data, val);
	}
	else if (val[data->i + 1] == '?')
	{
		printf("4\n");
		exp_exit_status_len(data);
	}
	else
	{
		printf("5\n");
		data->i++;
		data->exp_len++;
	}
}

/* Función que devuelve la longitud de un string tras expandirse
 * para hacer una correcta reserva de memoria. Expanded token len.

 * En caso de no haber un dólar se suma en uno tanto al iterador que recorre
 * el string como el entero a devolver. En caso de encontrar un dólar se
 * valorará el caso a través de la función format_len de forma similar al
 * especificador de formato en ft_printf.
 * 
 * - data->i recorre el valor a procesar.
 * - data->exp_len es el valor final del token expandido para una
 *   correcta reserva de memoria.
 */
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
	data->exp_len++; // para el null del final
	//printf("%i\n", data->exp_len);
	printf("data->exp_len: %i\n", data->exp_len);
	return (data->exp_len);
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