#include "shell.h"

t_sh_job sh_job(t_sh_job action)
{
	static t_sh_job flag = SH_JOB_NONE;
	if (action > SH_JOB_GET)
		flag = action;
	return (flag);
}