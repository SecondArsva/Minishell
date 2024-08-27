#include "../includes/minishell.h"

/*
 * La función del Autómata Finito Determinista o Finite State Machine,
 * de este en concreto, será la de hacer un análisis de la línea recibida
 * mediante la terminal y determinar si es sintácticamente válida o no.
 * 
 * Así si de simple. Aunque no va a resultar tan sencillo armarlo.
 * 
 * Estados:
 *	- Empty line (Estado inicial) (Válido)
 *	- Open double quotes (Inválido)
 *	- Open single quotes (Inválido)
 *	- Open Pipe (Invalido)
 *	- Open infile (Inválido)
 *	- Open outfile (Inválido)
 *	- Open heredoc (Inválido)
 *	- Open append (Inválido)
 *	- Reading text, not operator (Válido)
 *	- Spaces without words (¿Inválido?)
 *	- Spaces betwen words (Válido)
 *	- Invalid input (Inválido)
 *
 * La función "automata" devolverá un valor boleano con el que se determinará
 * si la línea tiene o no errores sintácticos, según el estado en el que finalize
 * para saber si se debe seguir con la ejecución de la minishell o no.
 * 
 * "evaluate" será la función de transición de estados que navegará sobre la
 * matriz de estados.
 * La matriz de estados es la variable "states" de la función "get_state"
 * Con "get_state" navegaremos la matriz mediante dos iteradores ("i" y "j")
 * que nos llevarán a un entero de la matriz el cuál será retornado.
 * Las filas de "state" simbolizan cada uno de los estados y las columnas
 * determinan a qué estado se ha de transicionar desde el estado actual
 * según el carácter encontrado.
 * Para ello es importante construir el alfabeto de la fsm siguiendo el mismo
 * orden que ocupen los carácteres a tratar según la columna en la que se
 * encuentre cada uno de ellos al haber creado la matriz de estados.
 * Esto es importante porque al usar "get_state" capturaremos el estado al que
 * transicionaremos desde el estado actual ("j") gracias a la función "get_idx"
 * que nos devolverá la posición del carácter coincidente dentro del
 * abecedario, teniendo así la columna exacta con el int del nuevo estado al
 * que deberemos movernos.
 * 
 * Mi función "get_idx" recibirá el abecedario y el carácter a encontrar.
 * Pondré un ejemplo simple con el abecedario simplificado:
 * 		Abecedario:	"\s\"\'|<>"
 * 		Índices:	  0 1 2345
 * "get_idx" busca en el abecedario el carácter a encontrar y devuélve el
 * indice en el que se encuentra dicho caracter. Como tengo 8 columnas
 * en la matriz de estados "state", en caso de no darse ninguna
 * coincidencia se devolvera el número 8 dando a entender que se encontró
 * un carácter válido que no es un operador.
 * Con esta lógica, puedo implementar los carácteres "\", "(", ")" y ";" y
 * tratar sus posiciones como un único índice devuelto con el que trabajar
 * en la matriz en una nueva columna referente a estos carácteres no
 * gestionables para poder tratar un nuevo estado inválido en caso de dar con
 * alguno de estos carácteres.
 * 		Abecedario:	"\s\"\'|<>\\();"
 * 		Arr_pos:	  0 1 2345 6789
 * En este ejemplo si en "get_idx" no se encuentra un caracter coincidente se
 * devolverá un 7 como el índice que refiere a esa lista de letras mayúsculas
 * o minisculas, números, guiónes, etc; en resumen, cualquier carácter válido que
 * no es un operador. Y en caso de encontrar "\", "(", ")" o ";" se hará que si
 * "char_pos" está entre el 6 y el 9 (ambos incluídos) se devuelva un 6 como
 * índice de la columna con la que se ha de capturar el valor.
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
	else if (i >= 6 && i <= 9)
		index = 6;
	else
		index = 7;
	return (index);
}

int	get_state(int i, int j)
{
	const int	states[][9] = {
//	 \s, ", ', |, <, >, i, c
	{ 0, 0, 0, 0, 0, 0, 0, 0}, //  0 - Empty line (Estado inicial) (Válido)
	{ 0, 0, 0, 0, 0, 0, 0, 0}, //  1 - Open double quotes (Inválido)
	{ 0, 0, 0, 0, 0, 0, 0, 0}, //  2 - Open single quotes (Inválido)
	{ 0, 0, 0, 0, 0, 0, 0, 0}, //  3 - Open Pipe (Invalido)
	{ 0, 0, 0, 0, 0, 0, 0, 0}, //  4 - Open infile (Inválido)
	{ 0, 0, 0, 0, 0, 0, 0, 0}, //  5 - Open outfile (Inválido)
	{ 0, 0, 0, 0, 0, 0, 0, 0}, //  6 - Open heredoc (Inválido)
	{ 0, 0, 0, 0, 0, 0, 0, 0}, //  7 - Open append (Inválido)
	{ 0, 0, 0, 0, 0, 0, 0, 0}, //  8 - Reading text, not operator (Válido)
	{ 0, 0, 0, 0, 0, 0, 0, 0}, //  9 - Spaces without words (¿Inválido?)
	{ 0, 0, 0, 0, 0, 0, 0, 0}, // 10 - Spaces betwen words (Válido)
	{ 0, 0, 0, 0, 0, 0, 0, 0}, // 11 - Invalid input (Inválido)
	{ 0, 0, 0, 0, 0, 0, 0, 0}, // 12 - No manage characters (Inválido)
	};

	return (states[i][j]);
}

int	evaluate(t_fsm *fsm)
{
	fsm->old_state = 0;
	fsm->i = 0;
	while (fsm->i < ft_strlen(fsm->cmd_line))
	{
		fsm->cur_state = get_state(fsm->cur_state, get_idx(fsm->alphabet, fsm->cmd_line[fsm->i]));
		fsm->old_state = fsm->cur_state;
		fsm->i++;
	}
}

void	init_automata(t_data *data, t_fsm *fsm)
{
	fsm->alphabet = ft_strdup(" \"'|<>\\();");
	fsm->errors = NULL;
	init_errors(data);
	fsm->cmd_line = ft_strdup(data->cmd_line);
	fsm->cur_state = 0;
	fsm->old_state = 0;
	fsm->i = 0;
	fsm->j = 0;
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
}