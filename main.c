#include "minishell.h"

volatile sig_atomic_t	g_last_signal = 0;


int	main(int argc, char **argv, char **envp)
{
	t_shell		shell;
	const char	*sh_name = msh_get_name(argc, argv);

	ft_bzero(&shell, sizeof(t_shell));
	if (msh_init(&shell, envp, sh_name) != 0)
		return (SH_GENERAL_ERROR);
	rl_catch_signals = 0;
	sh_setup_rl_hook(SH_JOB_RW);
	if (isatty(STDIN_FILENO))
		msh_run_interactive(sh_name, &shell);
	else
		msh_run_noninteractive(&shell);
}
