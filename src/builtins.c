#include "../includes/minishell.h"

void	my_env(t_data *data)
{
	env_print_list(data->env);
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
	while (to_eval[i])
	{
		if (!is_validenvchar(to_eval[i]))
			return (0);
		i++;
	}
	return (1);
}

// if_exists
// export_update_value
// ¿tendría que hacerle la gestión de comillas?

void	update_value(t_data *data, char *new_name, char *new_value)
{

}

void	export_new_node(t_data *data, char *new_name, char *new_value)
{
	t_env	*tmp;
	t_env	*new_node;

	tmp = data->env;
	new_node = safe_malloc(sizeof(t_env));
	new_node->var_name = safe_strdup(new_name);
	if (new_value)
		new_node->var_value = safe_strdup(new_value);
	else if (!new_value)
		new_node->var_value = NULL;
	// falta mandar el nuevo nodo al final de la lista. TODO
}

void	with_value(t_data *data, char *tok_value)
{
	char *new_name;
	char *new_value;

	new_name = export_grab_name(tok_value); // TODO
	new_value = export_grab_name(tok_value); // TODO
	if (var_exists(new_name))
		update_value(data, new_name, new_value);
	else
		export_new_node(data, new_name, new_value);
}

void	without_value(t_data *data, char *tok_value)
{
	if (var_exists(data, tok_value))
		return ;
	else if (is_validenvname(tok_value))
		export_new_node(data, tok_value, NULL); // TODO
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
	if (ft_strchr(tok_value, '=')) //Tiene un igual, por lo que se tiene que actualizar el valor en caso de existir, sino se crea
		with_value(data, tok_value); // TODO
	else
		without_value(data, tok_value);
}

void	my_export(t_data *data, t_token *current_token)
{
	t_token *tmp_token;

	tmp_token = current_token;
	while(tmp_token && tmp_token->type != PIPE)
	{
		if (tmp_token->type == COMMAND)
			export(data, tmp_token->value);
		tmp_token = tmp_token->next;
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

	tmp_token = current_token;
	while (tmp_token && tmp_token->type != PIPE)
	{
		if (tmp_token->type == COMMAND)
			unset(data, tmp_token->value);
		tmp_token = tmp_token->next;
	}
	*env_lst = data->env;
}