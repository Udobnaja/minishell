#include "executor_internal.h"

static int	exec_echo_is_newline(const char *str)
{
	return (str && str[0] == '-' && str[1] == 'n' && str[2] == '\0');
}
t_exec_status	echo(t_shell *sh, const t_cmd cmd)
{
	int i;
	int new_line;

	(void)sh;
	i = 1;
	new_line = 1;
	if (cmd.argv[i] && exec_echo_is_newline(cmd.argv[i]))
	{
		new_line = 0;
		i++;
	}
	while (cmd.argv[i])
	{
		ft_putstr_fd(cmd.argv[i], STDOUT_FILENO);
		if (cmd.argv[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (new_line)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (EXEC_OK);
}