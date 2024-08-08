#include "../includes/minishell.h"

/*
	El tokenizador da por supuesto que la línea de comandos
	introducida es correcta. Previo a él se usaría la
	máquina de estados finita que determinaría si la línea
	es correcta o no.

	Devolverá una lista enlazada tal y como acordé con Axel.
*/

/*
void	into_quotes_print(t_data *data)
{
	//data->i = 0;
	while (data->cmd_line[data->i])
	{
		if (data->cmd_line[data->i] == '\"' && !data->in_d_quot && !data->in_s_quot)
			data->in_d_quot = true;
		else if (data->cmd_line[data->i] == '\"' && data->in_d_quot)
			data->in_d_quot = false;
		else if (data->cmd_line[data->i] == '\'' && !data->in_s_quot && !data->in_d_quot)
			data->in_s_quot = true;
		else if (data->cmd_line[data->i] == '\'' && data->in_s_quot)
			data->in_s_quot = false;
		printf("%c - d: %i s: %i \n", data->cmd_line[data->i], data->in_d_quot, data->in_s_quot);
		data->i++;
	}
}
*/

void	tok_print_list(t_token *head)
{
	t_token	*tmp_lst;
	int		i;

	tmp_lst = head;
	i = 0;
	while (tmp_lst)
	{
		printf("node_pos: %i", i);
		printf("	type: %i", tmp_lst->type);
		if (tmp_lst->type == INFILE)
			printf(" infile");
		else if (tmp_lst->type == OUTFILE)
			printf(" outfile");
		else if (tmp_lst->type == APPEND)
			printf(" append");
		else if (tmp_lst->type == COMMAND)
			printf(" command");
		else if (tmp_lst->type == HEREDOC)
			printf(" heredoc");
		else if (tmp_lst->type == PIPE)
			printf(" pipe");
		printf("		my_address: %p", tmp_lst);
		printf("	next: %p	", tmp_lst->next);
		if (!tmp_lst->next)
			printf("	");
		printf("	value: %s", tmp_lst->value);
		printf("\n");
		tmp_lst = tmp_lst->next;
		i++;
	}
}

int	tok_list_size(t_token *list)
{
	int	total_nodes;

	total_nodes = 0;
	while (list)
	{
		total_nodes++;
		list = list->next;
	}
	return (total_nodes);
}

t_token	*tok_list_last(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	tok_update_tail(t_token *head, t_token *new_tail)
{
	t_token	*tmp;

	tmp = head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_tail;
}

int	tok_into_quotes(t_data *data)
{
	if (data->cmd_line[data->j] == '\"' && !data->in_d_quot && !data->in_s_quot)
		data->in_d_quot = true;
	else if (data->cmd_line[data->j] == '\"' && data->in_d_quot)
		data->in_d_quot = false;
	else if (data->cmd_line[data->j] == '\'' && !data->in_s_quot && !data->in_d_quot)
		data->in_s_quot = true;
	else if (data->cmd_line[data->j] == '\'' && data->in_s_quot)
		data->in_s_quot = false;
	if (data->in_d_quot == true || data->in_s_quot == true)
		return (1);
	else
		return (0);
}

char *tok_value(t_data *data)
{
	char	*value;

	while (data->cmd_line[data->i] == ' ')
		data->i++;
	data->j = data->i;
	while (data->cmd_line[data->j])
	{
		if (tok_into_quotes(data))
			data->j++;
		else if (!tok_into_quotes(data) && ft_strchr(" <>|", data->cmd_line[data->j]))
			break;
		else if (!tok_into_quotes(data))
			data->j++;
	}
	value = ft_substr(data->cmd_line, data->i, (data->j) - data->i);
	data->i = data->j;
	return (value);
}

/*
 * Avanza el iterador en caso de ser necesario según
 * el tipo y recoge el valor correspondiente.
*/
char *tok_grab_value(t_data *data, t_type opcode)
{
	if (opcode == PIPE)
		data->i++;
	else if (opcode == INFILE || opcode == OUTFILE)
		data->i++;
	else if (opcode == HEREDOC || opcode == APPEND)
		data->i = data->i + 2;
	if (opcode == PIPE)
		return (NULL);
	else
		return (tok_value(data));
	return ("fool return");
}

// Incrementa el total de nodos según su tipo y así saber cuantos hay.
// Un contador, vamos...
void	tok_increase_node_type(t_data *data, t_type opcode)
{
	if (opcode == INFILE)
		data->exec->n_infiles++;
	else if (opcode == OUTFILE)
		data->exec->n_outfiles++;
	else if (opcode == APPEND)
		data->exec->n_appends++;
	else if (opcode == COMMAND)
		data->exec->n_commands++;
	else if (opcode == HEREDOC)
		data->exec->n_heredocs++;
	else if (opcode == PIPE)
		data->exec->n_pipes++;
}

// crea el nodo según el tipo
void	tok_new_node(t_data *data, t_type opcode)
{
	t_token *new_node;

	new_node = NULL;
	new_node = (t_token *) safe_malloc(sizeof(t_token));
	new_node->type = opcode;
	new_node->value = tok_grab_value(data, opcode);
	new_node->next = NULL;
	if (!data->tokens)
		data->tokens = new_node;
	else
		tok_update_tail(data->tokens, new_node);
	tok_increase_node_type(data, opcode);
}

// <
// <<
// >
// >>
// |
// ""
// ''
// \S
// char

	// build the list //list_building();
	// reorganize the CMD nodes //list_reorganize();
	// expand the enviroment variables //list_expander();
void	tokenizer(t_data *data)
{
	data->i = 0;
	data->j = 0;

	while (data->cmd_line[data->i])
	{
		if (data->cmd_line[data->i] == ' ')
			data->i++;
		else if (data->cmd_line[data->i] == '|')
			tok_new_node(data, PIPE);
		else if (data->cmd_line[data->i] == '<' && data->cmd_line[data->i+1] == '<')
			tok_new_node(data, HEREDOC);
		else if (data->cmd_line[data->i] == '>' && data->cmd_line[data->i+1] == '>')
			tok_new_node(data, APPEND);
		else if (data->cmd_line[data->i] == '<')
			tok_new_node(data, INFILE);
		else if (data->cmd_line[data->i] == '>')
			tok_new_node(data, OUTFILE);
		else
			tok_new_node(data, COMMAND);
	}
}
