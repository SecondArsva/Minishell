#ifndef MINISHELL_H
# define MINISHELL_H

# include <stddef.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
/*
typedef enum	e_states
{
	EMPTY,
	SPACE,			// " "
	DOBLE_QUOTES,	// ""
	SINGLE_QUOTE,	// ''
	PIPE,			// |
	INFILE,			// <
	OUTFILE,		// >
	HEREDOC,		// <<
	APPEND,			// >>
}				t_states;
*/

typedef enum	e_type
{
	INFILE,
	OUTFILE,
	APPEND,
	COMMAND,
	HEREDOC,
	PIPE,
}				t_type;

/*
typedef struct s_token
{
	t_type			type;
	char			*filename;
	char			**command;
	char			*end_of_file;
	struct s_token	*next;
}				t_token;
*/

typedef struct	s_token
{
	t_type			type;
	char			*value; // for debbuging
	//char			**value;
	struct s_token	*next;
}				t_token;

typedef struct	s_fsm
{
	// alfabeto
	// estado previo
	// estado actual
}				t_fsm;

typedef struct s_data
{
	char	*cmd_line;
	char	**env;
	int		i;
	int		j;
	int		k;
	int		l;
	int		m;
	bool	in_s_quot;
	bool	in_d_quot;
	bool	quoted;
	t_token	*tokens;
	t_fsm	*fsm;
}				t_data;

//	### - SAFE ALLOC - ###
void	*safe_malloc(size_t bytes);
void	*safe_calloc(size_t count, size_t size);
char	**safe_matrixdup(char **original);

//	### - TOKENIZER - ###
void	tokenizer(t_data *data);
void	tok_print_list(t_token *head);
int		tok_into_quotes(t_data *data);

//	### - EXPANDER - ###
void	expander(t_data *data);

#endif