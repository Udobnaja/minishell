#include "minishell.h"

const char *get_prompt(char *default_name)
{
	(void) (default_name);
	return ("minishell $ ");
}

int main(int argc, char **argv, char **envp)
{
	t_shell shell;
	const char *sh_name;

	t_pipeline pipeline;
	t_cmd *cmd = malloc(sizeof(t_cmd));
	char **argv_cmd = malloc(3 * sizeof(char *));
	t_cmd **cmds = malloc(sizeof(t_cmd *));
	cmd->name = "unset";
	argv_cmd[0] = "unset";
	argv_cmd[1] = NULL;
	//argv_cmd[2] = NULL;
	cmd->argv = argv_cmd;
	cmd->redirect_list = NULL;
	cmd->builtin_kind = BUILTIN_EXPORT;
	cmds[0] = cmd;

	pipeline.count = 1;
	pipeline.cmds = cmds;

	if (argc > 0 && argv && argv[0] && argv[0][0] != '\0')
		sh_name = argv[0];
	else
		sh_name = SHELL_NAME;
	ft_bzero(&shell, sizeof(t_shell));
	if (sh_env_init(&shell, envp, sh_name) != ENV_OK)
		return (1);
	
	char *line;

	while(1)
	{
		line = readline(get_prompt(argv[0]));
		if (!line)
			break;
		if (*line)
		{
			add_history(line);
			sh_parse(line);
		}		
		mock_exec(&shell, &pipeline);	
		free(line);
		line = NULL;
	}
	env_destroy(&shell.env_store);
	return (0);
}

