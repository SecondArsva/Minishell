#include "../includes/minishell.h"

/*
	El tokenizador da por supuesto que la línea de comandos
	introducida es correcta. Previo a él se usaría la
	máquina de estados finita que determinaría si la línea
	es correcta o no.

	Devolverá una lista enlazada tal y como acordé con Axel.
*/

void	tok_print_list(t_token *head)
{
	t_token	*tmp_lst;
	int		i;

	tmp_lst = head;
	i = 0;
	while (tmp_lst)
	{
		printf("node pos: %i", i);
		printf("	type: %i", tmp_lst->type);
		printf("		my address: %p", tmp_lst);
		printf("	next: %p", tmp_lst->next);
		// print value here at the end
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

char *tok_file_name(t_data *data)
{
	
}

char *tok_grab_value(t_data *data, t_type opcode)
{
	if (opcode == PIPE)
		data->i++;
	else if (opcode == INFILE || opcode == OUTFILE)
		data->i++;
	else if (opcode == HEREDOC || opcode == APPEND)
		data->i + 2;

	if (opcode == PIPE)
		return (NULL);
	else if (opcode == INFILE || opcode == OUTFILE || opcode == APPEND)
		return (tok_file_name(data));
	else if (opcode == HEREDOC)
		return (tok_end_of_file(data));
	else if (opcode == COMMAND);
		return (tok_command(data));
	return ("fool return")
}

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

	// for debug
	//while (data->cmd_line[data->i])
	//{
	//	tok_new_node(data, INFILE);
	//	data->i++;
	//}
	
	while (data->cmd_line[data->i])
	{
		if (data->cmd_line[data->i] == ' ')
			data->i++;
		else if (data->cmd_line[data->i] == '|')
			tok_new_node(data, PIPE);
		else if (data->cmd_line[data->i] == '<')
			tok_new_node(data, INFILE);
		else if (data->cmd_line[data->i] == '>')
			tok_new_node(data, OUTFILE);
		else if (data->cmd_line[data->i] == '<' && data->cmd_line[data->i++] == '<')
			tok_new_node(data, HEREDOC);
		else if (data->cmd_line[data->i] == '>' && data->cmd_line[data->i++] == '>')
			tok_new_node(data, APPEND);
		else
			tok_new_node(data, COMMAND);
	}
}

/*
void	into_quotes(t_data *data)
{
	data->i = 0;
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

/*
int main(int argc, char **argv, char **env)
{
	t_data	*data;
	char	*cmd_line = "<infile <   '\"$USER\"' 'cat' -e | \"cat\" >> \"'outfile'$USER\"";

	if (argc != 1)
	{
		return (printf("only one argument\n"));
		ft_print_matrix(argv, "argv");
	}
	ft_printf("Hola\n");
	data = safe_malloc(sizeof(t_data));
	init_data(data, cmd_line, env);
	ft_printf("%s\n", data->cmd_line);
	into_quotes(data);
	//ft_print_matrix(data->env, "data->env");
	printf("%s\n", argv[1]);
	//tokenizer();
	return (0);
}
*/