#include "shell.h"

void	sh_shell_signals(void)
{
	sh_set_signal_handler(SIGQUIT, SIG_IGN);
	if (isatty(STDIN_FILENO))
	{
		sh_set_signal_handler(SIGINT, sh_sigint_sighandler);
		sh_set_signal_handler(SIGTERM, SIG_IGN);
	}
}

void	sh_heredoc_signals(void)
{
	sh_set_signal_handler(SIGQUIT, SIG_IGN);
	sh_set_signal_handler(SIGINT, sh_sigint_sighandler);
}

void	sh_childprocess_signals(void)
{
	sh_set_signal_handler(SIGINT,  SIG_DFL);
	sh_set_signal_handler(SIGQUIT, SIG_DFL);
	sh_set_signal_handler(SIGTERM, SIG_DFL);
}

void	sh_empty_signals(void)
{
	sh_set_signal_handler(SIGQUIT, SIG_IGN);
	sh_set_signal_handler(SIGINT, SIG_IGN);
	sh_set_signal_handler(SIGTERM, SIG_IGN);
}
