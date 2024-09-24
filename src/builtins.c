#include "../includes/minishell.h"

/*
	[v] TODO export sin argumentos

	[v] TODO gestión de comillas como al exportar así como lo hago en la
 	creación del eviroment:

	- Si la cadena "new_value" no empieza con comillas dobles, se le ponen
	  comillas dobles en sus extremos. Principio y final.
	- Si empieza con comillas dobles, se ponen comillas simples. Abrazando al
	  conjunto de la cadena respetando las comillas dobles.

		export	A=A		>>> A="A"
		export	B="B"	>>> B='"B"'
	
	- Básicamente hay que crear y adaptar una nueva función basada
	  en "env_quoting_value" de "init_env.c" e implementarla en "grab_value".
*/


void	my_env(t_data *data)
{
	t_env	*tmp;
	char	*cleaned_value;

	cleaned_value = NULL;
	tmp = data->env;
	while (tmp)
	{
		if (tmp->var_value[0] == '\"')
			cleaned_value = safe_strtrim(tmp->var_value, "\"");
		else if (tmp->var_value[0] == '\'')
			cleaned_value = safe_strtrim(tmp->var_value, "\'");
		printf("%s=%s\n", tmp->var_name, cleaned_value);
		free(cleaned_value);
		tmp = tmp->next;
	}
}

/*
 * Export exporta todo lo que se encuentre tras él, a excepción de las
 * redirecciones.
 * Parando obviamente en el pipe o cuando se termine la línea de comandos...
 * 
 * Tengo que tratar de exportar tras el nodo que contenga "export" todos los
 * token COMMAND que encuentre hasta que se termine la lista o llegue a un
 * nodo de tipo pipe. Pero, claro no puedo exportar lo que haya antes del export
 * 
 * al usar varios export separados por pipes no se exporta nada...
 * Cuando falla alguna redirección tampoco se hace el export.
*/

int	var_exists(t_data *data, char *to_find)
{
	t_env	*tmp;

	tmp = data->env;
	while (tmp)
	{
		if (!ft_strncmp(to_find, tmp->var_name, ft_strlen(tmp->var_name) + 1))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	is_validenvname(char *to_eval)
{
	int	i;

	i = 0;
	if (to_eval[0] == '\0')
		return (0);
	while (to_eval[i])
	{
		if (!is_validenvchar(to_eval[i]))
			return (0);
		i++;
	}
	return (1);
}

/*
// if_exists
// export_update_value
// ¿tendría que hacerle la gestión de comillas?
*/
void	update_node(t_data *data, char *to_find, char *new_value)
{
	t_env	*tmp;

	tmp = data->env;
	while (tmp)
	{
		if (!ft_strncmp(to_find, tmp->var_name, strlen(tmp->var_name) + 1))
			break ;
		tmp = tmp->next;
	}
	if (tmp->var_value)
		free(tmp->var_value);
	tmp->var_value = new_value;
	free(to_find);
}

void	new_node(t_data *data, char *new_name, char *new_value)
{
	t_env	*new_node;

	new_node = safe_malloc(sizeof(t_env));
	if (!new_value)
	{
		new_node->var_name = safe_strdup(new_name);
		new_node->var_value = safe_strdup("\"\"");
	}
	else if (new_value)
	{
		new_node->var_name = new_name;
		new_node->var_value = new_value;
	}
	new_node->next = NULL;
	if (!data->env)
		data->env = new_node;
	else
		env_update_tail(data->env, new_node);
}

char	*grab_name(char *tok_value)
{
	int	i;

	i = 0;
	while (tok_value[i])
	{
		if (tok_value[i] == '=')
			break ;
		i++;
	}
	return(safe_substr(tok_value, 0, i));
}

char	*in_quotes(char *tok_value, int start, size_t len, t_quote opcode)
{
	char *value;
	char *quote;
	char *first_join;
	char *last_join;

	value = safe_substr(tok_value, start, len);
	if (opcode == SINGLE)
		quote = "'";
	else
		quote = "\"";
	first_join = safe_strjoin(quote, value);
	last_join = safe_strjoin(first_join, quote);
	free(value);
	free(first_join);
	return(last_join);
}

char	*quoting_value(char *tok_value, unsigned int start, size_t len)
{
	if (tok_value[start] == '"')
		return (in_quotes(tok_value, start, len, SINGLE));
	else
		return (in_quotes(tok_value, start, len, DOUBLE));
}

char	*grab_value(char *tok_value)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (tok_value[i])
	{
		if (tok_value[i] == '=')
			break ;
		i++;
	}
	j = i;
	while (tok_value[j])
		j++;
	return(quoting_value(tok_value, i + 1, (j - i) + 1));
}

void	with_value(t_data *data, char *tok_value)
{
	char	*new_name;
	char	*new_value;

	new_name = grab_name(tok_value);
	new_value = grab_value(tok_value);
	if (!is_validenvname(new_name))
		{
			printf("Minishell: export: '%s': invalid identifier\n", tok_value);
			exit(1);
		}
	if (var_exists(data, new_name))
		update_node(data, new_name, new_value);
	else
		new_node(data, new_name, new_value);
}

void	without_value(t_data *data, char *tok_value)
{
	if (var_exists(data, tok_value))
		return ;
	else if (is_validenvname(tok_value))
		new_node(data, tok_value, NULL);
	else
	{
		printf("Minishell: export: '%s': invalid identifier\n", tok_value);
		exit(1);
	}
}

// Export
// Encapsulo lo que encuentre como nombre y valor
// Verifico que todos los carácteres que componen el nombre del valor sean válidos.
	// si hay alguno que no lo sea lanzo un error y exit
// Primero busco si hay una variable con el mismo nombre y actualizo su valor en caso de que ya exista
// Si no existe creo un nuevo nodo al final de la lista con el nombre y el valor recibidos.
void	export(t_data *data, char *tok_value)
{
	if (ft_strchr(tok_value, '='))
		with_value(data, tok_value);
	else
		without_value(data, tok_value);
}

int	something_to_work(t_token *list)
{
	t_token	*tmp;

	tmp = list;
	while (tmp)
	{
		if (tmp->type == COMMAND)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

void	no_args_print(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		printf("declare -x %s=%s\n", tmp->var_name, tmp->var_value);
		tmp = tmp->next;
	}
}

void	my_export(t_data *data, t_token *current_token)
{
	t_token *tmp_token;

	tmp_token = current_token;
	if (!something_to_work(tmp_token))
		no_args_print(data->env);
	else
	{
		while(tmp_token && tmp_token->type != PIPE)
		{
			if (tmp_token->type == COMMAND)
				export(data, tmp_token->value);
			tmp_token = tmp_token->next;
		}
	}
}

// UNSET ----------------------------------------------------------------------
void	unset(t_data *data, char *to_find)
{
	t_env	*this;
	t_env	*prev;

	this = data->env;
	if (!ft_strncmp(to_find, this->var_name, ft_strlen(this->var_name) + 1))
	{
		data->env = data->env->next;
		free(this);
	}
	else
	{
		while (this)
		{
			if (!ft_strncmp(to_find, this->var_name,
				ft_strlen(this->var_name) + 1))
			{
				prev->next = this->next;
				free(this);
			}
			prev = this;
			this = this->next;
		}
	}

}

void	my_unset(t_data *data, t_token *current_token, t_env **env_lst)
{
	t_token *tmp_token;

	if (!current_token)
		return ;
	tmp_token = current_token;
	while (tmp_token && tmp_token->type != PIPE)
	{
		if (tmp_token->type == COMMAND)
			unset(data, tmp_token->value);
		tmp_token = tmp_token->next;
	}
	*env_lst = data->env;
}