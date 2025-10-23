#include "errors_internal.h"

const char	*err_heredoc_msg(int code)
{
	static const char	*map[] = {
	[HEREDOC_ABORTED] = "",
	[HEREDOC_CREATE_FAIL] = "cannot create temp file for here-document: \
		Too many open files",
	[HEREDOC_EXCEED_LIMIT] = "maximum here-document count exceeded",
	[HEREDOC_ALLOC_ERROR] = "Cannot allocate memory",
	[HEREDOC_WRITE_ERROR] = "Input/output error",
	[HEREDOC_ERRNO] = "%s"};
	const int			size = sizeof(map) / sizeof(*map);

	if (code < 0 || code >= size)
		return (NULL);
	return (map[code]);
}
