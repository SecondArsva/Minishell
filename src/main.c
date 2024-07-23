#include "../includes/minishell.h"

void	init_data(t_data *data, char **env)
{
	data->tokens = NULL;
	data->fsm = NULL;
	data->i = 0;
	data->j = 0;
	data->env = safe_matrixdup(env);
}

void	print_data(t_data *data)
{
	printf("token: %p\n", data->tokens);
	printf("fsm: %p\n", data->fsm);
	printf("i: %i\n", data->i);
	printf("j: %i\n", data->j);
	ft_print_matrix(data->env, "env");
}

/*
 * TODO barras invertidas
*/

int	main(int argc, char **argv, char **env)
{
	t_data	*data;
	char	*cmd_line;

	(void)argc;
	(void)argv;
	data = safe_malloc(sizeof(t_data));
	init_data(data, env);
	cmd_line = readline("Minishell$ ");
	printf("cmd_line: %s\n", cmd_line);
	data->cmd_line = ft_strdup(cmd_line);
	tokenizer(data);
	tok_print_list(data->tokens);
	return (0);
}