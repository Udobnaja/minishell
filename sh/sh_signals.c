#include "shell.h"

static void sh_sigint_sighandler(int signo)
{
    g_last_signal = signo;
    write(STDOUT_FILENO, "\n", 1);
}

static void sh_set_signal_handler(int sig, void (*handler)(int))
{
	struct sigaction sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	sigaction(sig, &sa, NULL);
}

void sh_shell_signals(void)
{
	sh_set_signal_handler(SIGQUIT, SIG_IGN);
	if (isatty(STDIN_FILENO))
	{
		sh_set_signal_handler(SIGINT, sh_sigint_sighandler);
		sh_set_signal_handler(SIGTERM, SIG_IGN);
	}
}

void sh_heredoc_signals(void)
{
    sh_set_signal_handler(SIGQUIT, SIG_IGN);
	sh_set_signal_handler(SIGINT, sh_sigint_sighandler);
}

void sh_childprocess_signals(void)
{
    sh_set_signal_handler(SIGINT, SIG_DFL);
    sh_set_signal_handler(SIGQUIT, SIG_DFL);
}