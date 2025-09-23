#ifndef PIPELINE_H
# define PIPELINE_H

# include <stddef.h>

typedef enum e_io_stream
{
	IO_STDIN = 0,
	IO_STDOUT = 1,
	IO_STDERR = 2
}	t_io_stream;

typedef enum e_redirect_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redirect_type;

typedef union u_redirect_target
{
	char	*path; 
    int		fd;
}	t_redirect_target;

typedef struct s_redirect
{
	t_io_stream			stream;
	t_redirect_type		type;
	t_redirect_target	target;
	struct s_redirect	*next;
}	t_redirect;

typedef enum e_builtin
{
	BUILTIN_NONE,
	BUILTIN_ECHO,
	BUILTIN_CD,
	BUILTIN_PWD,
	BUILTIN_EXPORT,
	BUILTIN_UNSET,
	BUILTIN_ENV,
	BUILTIN_EXIT
}	t_builtin;

typedef struct s_cmd
{
	char		*name;
	char		**argv;
	t_redirect	*redirect_list;
	t_builtin	builtin_kind;
	size_t		argc;
    size_t		capacity;
}	t_cmd;

typedef struct s_pipeline
{
	t_cmd	**cmds;
	size_t	count;
}	t_pipeline;

void	pipeline_destroy(t_pipeline *pipeline);
int		pipeline_init(const size_t cmds_count, t_pipeline *pipeline);
int		pipeline_push_cmd_argv(t_cmd *cmd, char *arg);
void	pipeline_push_redirect(t_cmd *cmd, t_redirect *node);

#endif
