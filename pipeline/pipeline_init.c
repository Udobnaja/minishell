#include "pipeline_internal.h"

int	pipeline_init(const size_t cmds_count, t_pipeline *pipeline)
{
	size_t	i;

	i = 0;
	pipeline->cmds = ft_calloc((size_t)cmds_count, sizeof *pipeline->cmds);
	if (!pipeline->cmds)
		return (0);
	pipeline->count = cmds_count;
	while (i < cmds_count)
	{
		pipeline->cmds[i] = ft_calloc(1, sizeof *pipeline->cmds[i]);
		if (!pipeline->cmds[i])
		{
			while (i--)
				free(pipeline->cmds[i]);
			pipeline_destroy(pipeline);	
			pipeline->cmds = NULL;
			return (0);
		}
		i++;
	}		
	return (1);
}