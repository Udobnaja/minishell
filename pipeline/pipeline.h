#ifndef PIPELINE_H
# define PIPELINE_H

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
}	t_cmd;

typedef struct s_pipeline
{
	t_cmd	**cmds;
	int		count;
}	t_pipeline;

#endif
