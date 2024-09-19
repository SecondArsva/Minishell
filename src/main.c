#include "../includes/minishell.h"

void	init_exec(t_data *data)
{
	data->exec = (t_exec *) safe_malloc(sizeof(t_exec));
	data->exec->n_infiles = 0;
	data->exec->n_outfiles = 0;
	data->exec->n_appends = 0;
	data->exec->n_commands = 0;
	data->exec->n_heredocs = 0;
	data->exec->n_pipes = 0;
}

void	init_data(t_data *data, char **env, t_env **env_lst)
{
	data->tokens = NULL;
	data->fsm = NULL;
	data->i = 0;
	data->j = 0;
	data->exp_len = 0;
	data->exp_str = NULL;
	data->in_s_quot = 0;
	data->in_d_quot = 0;
	data->quoted = 0;
	data->exit_status = 0;
	init_exec(data);
	data->fsm = NULL;
	if (!(*env_lst))
	{
		init_env(data, env);
		*env_lst = data->env;
	}
	else
		data->env = *env_lst;
}

void	print_data(t_data *data)
{
	printf("token: %p\n", data->tokens);
	printf("fsm: %p\n", data->fsm);
	printf("i: %i\n", data->i);
	printf("j: %i\n", data->j);
}

void	tok_print_total_node_types(t_exec *exec)
{
	printf("\n- Node Type Counter -\n");
	printf("	INF: %i", exec->n_infiles);
	printf("	OUT: %i", exec->n_outfiles);
	printf("	APP: %i", exec->n_appends);
	printf("	COM: %i", exec->n_commands);
	printf("	HER: %i", exec->n_heredocs);
	printf("	PIP: %i\n\n", exec->n_pipes);
}

void	env_print_list(t_env *head)
{
	t_env	*tmp;

	tmp = head;
	while (tmp)
	{
		printf("%s=%s\n", tmp->var_name, tmp->var_value);
		tmp = tmp->next;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_data	*data;
	char	*cmd_line;
	t_env	*env_lst;

	(void)argc;
	(void)argv;
	env_lst = NULL;
	data = safe_malloc(sizeof(t_data));
	init_data(data, env, &env_lst);
	cmd_line = readline("Minishell$ ");
	data->cmd_line = safe_strdup(cmd_line);
	if (!automata(data))
		exit(2);
	tokenizer(data);
//	tok_print_list(data->tokens);
	//tok_print_total_node_types(data->exec);
//	env_print_list(data->env);
	expander(data);
	tok_print_list(data->tokens);
	quoter(data);
	tok_print_list(data->tokens);
	printf("dat: %p\n", data->env);
	printf("env: %p\n", env_lst);
	
	printf("Builtins\n");
	my_env(data);
	my_unset(data, data->tokens, &env_lst);
	printf("\n\n[x] UNSETED [x]\n\n");
	my_env(data);
	printf("dat: %p", data->env);
	printf("env: %p", env_lst);
	return (0);
}
