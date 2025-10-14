#include "shell.h"

static int	sh_rl_interactive_hook(void)
{
	if (g_last_signal == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
	}

	return 0;
}

static int	sh_rl_heredoc_hook(void)
{
	if (g_last_signal == SIGINT)
        write(STDOUT_FILENO, "\n", 1);

	return 0;
}

void sh_setup_rl_hook(t_sh_job job)
{
    if (job == SH_INTERACTIVE)
    {
        rl_event_hook = sh_rl_interactive_hook;
        sh_shell_signals();
    }
    else if (job == SH_HEREDOC)
    {
        rl_event_hook = sh_rl_heredoc_hook;
        sh_heredoc_signals();
    }
    else if (job == SH_CHILD)
    {
        rl_event_hook = NULL;
        sh_childprocess_signals();
    }
}