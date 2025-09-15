#include "shell.h"

static int	sh_cpy_tmpdir(char *dst, const char *src, size_t max_name_size);

int sh_get_tmpdir(const t_env_store *env, char *tmpdir, size_t max_name_size)
{
	const char	*temp_dirs[5];
	const char	*env_tmp_dir = env_get_value(env, "TMPDIR");
	size_t		i;
	size_t		j;

	i = 0;
	if (env_tmp_dir && env_tmp_dir[0])
		temp_dirs[i++] = env_tmp_dir;
	temp_dirs[i++] = "/tmp";
    temp_dirs[i++] = "/var/tmp";
    temp_dirs[i++] = "/usr/tmp";
    temp_dirs[i++] = ".";
	j = 0;
	while (j < i)
	{
		if (u_file_iswxdir(temp_dirs[j]))
			return (sh_cpy_tmpdir(tmpdir, temp_dirs[j], max_name_size));
		j++;
	}
	errno = ENOENT;
    return (-1);
}

static int	sh_cpy_tmpdir(char *dst, const char *src, size_t max_name_size)
{
	size_t len;
	
	len = ft_strlen(src);
    while (len > 1 && src[len - 1] == '/')
		len--;
    if (len + 1 > max_name_size) {
		errno = ENAMETOOLONG;
		return -1;
	}
    ft_memcpy(dst, src, len);
    dst[len] = '\0';
    return (0);
}
