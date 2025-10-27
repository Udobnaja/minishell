#include "shell.h"

static void	sh_core_dump_reporter(int wst);

void	sh_sigint_sighandler(int signo)
{
	g_last_signal = signo;
	rl_done = 1;
}

void	sh_set_signal_handler(int sig, void (*handler)(int))
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	sigaction(sig, &sa, NULL);
}

void	sh_report_wait_signal(int wst)
{
	if (!WIFSIGNALED(wst))
		return ;
	if (WTERMSIG(wst) == SIGQUIT)
		sh_core_dump_reporter(wst);
}

#ifdef WCOREDUMP

static void	sh_core_dump_reporter(int wst)
{
	if (WCOREDUMP(wst))
		write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	else
		write(STDERR_FILENO, "Quit\n", 5);
}
#else

static void	sh_core_dump_reporter(int wst)
{
	(void)wst;
	write(STDERR_FILENO, "Quit\n", 5);
}
#endif