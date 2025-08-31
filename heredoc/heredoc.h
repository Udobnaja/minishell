#ifndef HEREDOC_H
# define HEREDOC_H

typedef enum e_heredoc_status {
	HEREDOC_OK = 0,
	HEREDOC_ABORTED, // TODO: not sure that it is a error
    HEREDOC_EXCEED_LIMIT,
    HEREDOC_CREATE_FAIL
}	t_heredoc_status;

#endif