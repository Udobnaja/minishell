#include "shell.h"

#if defined(ECHOCTL)

void	sh_echoctl_off(struct termios *t)
{
	t->c_lflag &= ~ECHOCTL;
}

void	sh_echoctl_on(struct termios *t)
{
	t->c_lflag |= ECHOCTL;
}
#else

void	sh_echoctl_off(struct termios *t)
{
	(void)t;
}

void	sh_echoctl_on(struct termios *t)
{
	(void)t;
}
#endif