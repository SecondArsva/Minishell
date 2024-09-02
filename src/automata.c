#include "../includes/minishell.h"

/* 
 * Comprueba el comportamiento de bash ante los carácteres que no se
 * gestionan en caso de estar entrecomillados para actualizar la matriz de
 * estados en caso de ser necesario. Añade el & a estos carácteres.
 * No lo había tenido en cuenta hasta que no me lo ha dicho Axel.
 * echo "\();&"
 *       \();&
 * echo '\();&'
 * 		 \();&
 * Actúan de la misma forma. Si están entre comillas se interpretan como
 * carácteres planos. Así que he de añardir el "&" al abecedario y gestionar
 * correctamente estos carácteres en caso de encontrarlos entrecomillados
 * cambiando algunos indices de la matriz de estados y ampliar "get_idx"
*/
int	get_idx(char *alphabet, char anal)
{
	int	i;
	int index;

	i = 0;
	index = 0;
	while (alphabet[i])
	{
		if (alphabet[i] == anal)
			break ;
		i++;
	}
	if (i >= 0 && i <= 5)
		index = i;
	else if (i >= 6 && i <= 10)
		index = 6;
	else
		index = 7;
	return (index);
}

int	get_state(int i, int j)
{
	const int	states[][9] = {
//	 \s, ', ", |, <, >, invalid, anychar
//
	{ 0, 1, 2, 10, 4, 5, 11, 12},		//  0 - Empty line (Estado inicial) (Válido)	[X]
	{ 1, 9, 1, 1, 1, 1, 1, 1},			//  1 - Open Single quotes (Inválido)
	{ 2, 2, 9, 2, 2, 2, 2, 2},			//  2 - Open Double quotes (Inválido)
	{ 8, 1, 2, 11, 4, 5, 11, 12},		//  3 - Open Pipe (Invalido)
	{ 8, 1, 2, 10, 6, 10, 11, 12},		//  4 - Open Infile (Inválido)
	{ 8, 1, 2, 10, 10, 7, 11, 12},		//  5 - Open Outfile (Inválido)
	{ 8, 1, 2, 10, 10, 10, 11, 12}, 	//  6 - Open Heredoc (Inválido)
	{ 8, 1, 2, 10, 10, 10, 11, 12}, 	//  7 - Open Append (Inválido)
	{ 8, 1, 2, 10, 10, 10, 11, 12}, 	//  8 - Space after open operator (Inválido)
	{ 9, 1, 2, 3, 4, 5, 11, 12},		//  9 - Spaces between words (Válido)			[X]
	{ 10, 10, 10, 10, 10, 10, 10, 10}, 	// 10 - Syntax Error - Invalid input (Inválido)
	{ 10, 10, 10, 10, 10, 10, 10, 10}, 	// 11 - No manage characters (Inválido)
	{ 9, 1, 2, 3, 4, 5, 11, 12}, 		// 12 - Reading text, not operator (Válido)		[X]
	};

	return (states[i][j]);
}

int	evaluate(t_fsm *fsm)
{
	fsm->old_state = 0;
	fsm->i = 0;
	while (fsm->i < (int)ft_strlen(fsm->cmd_line))
	{
		fsm->cur_state = get_state(fsm->cur_state,
				get_idx(fsm->alphabet, fsm->cmd_line[fsm->i]));
		fsm->old_state = fsm->cur_state;
		fsm->i++;
	}
	return (fsm->cur_state);
}

void	init_automata(t_data *data, t_fsm *fsm)
{
	fsm->alphabet = safe_strdup(" '\"|<>\\();&");
	fsm->cmd_line = safe_strdup(data->cmd_line);
	fsm->cur_state = 0;
	fsm->old_state = 0;
	fsm->i = 0;
	fsm->get_state = get_state;
}

int	automata(t_data *data)
{
	t_fsm	*fsm;
	int		final_state;

	final_state = 0;
	fsm = safe_calloc(1, sizeof(t_fsm));
	data->fsm = fsm;
	init_automata(data, data->fsm);
	final_state = evaluate(data->fsm);
	if (final_state == 0 || final_state == 9 || final_state == 12)
		return (1);
	else
		return (ft_printf_error("Minishell: syntax error\n"), 0);
}