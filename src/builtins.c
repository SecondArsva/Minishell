#include "../includes/minishell.h"

void	my_env(t_data *data)
{
	env_print_list(data->env);
}

/*
void	export(t_data *data, t_token *tmp_token)
{

}

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
void	my_export(t_data *data, t_token *actual_token, t_env **env_lst)
{
	t_token *tmp_token;

	tmp_token = actual_token;
	while (tmp_token && tmp_token->type != PIPE)
	{
		if (tmp_token->type == COMMAND)
			export(data, tmp_token);
		tmp_token = tmp_token->next;
	}
	*env_lst = data->env;
}
*/

// ----------------------------------------------------------------------------

void	unset(t_data *data, char *to_find)
{
	t_env *this;
	t_env *prev;

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