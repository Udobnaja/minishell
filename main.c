#include "minishell.h"

char *get_prompt()
{
	return ("minishell$");
}

int main ()
{
	char *line;

	while(1)
	{
		line = readline(get_prompt());
		if (!line)
		{
			return (0);
		} else 
		{
			if (*line)
    			add_history(line);
			free(line);
		}
	}
}