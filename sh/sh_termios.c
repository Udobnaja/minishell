#include "shell.h"

int		sh_termios_apply(void)
{
    struct termios t;
	int ret;

    ret = tcgetattr(STDIN_FILENO, &t);
    if (ret == -1)
		return (-1);
    sh_echoctl_off(&t);
    ret = tcsetattr(STDIN_FILENO, TCSANOW, &t);
	if (ret == -1)
		return (-1);
	return (0);
}

void	sh_termios_restore(void)
{
    struct termios t;
	int ret;

    ret = tcgetattr(STDIN_FILENO, &t);
    if (ret == -1)
		return ;
    sh_echoctl_on(&t);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}