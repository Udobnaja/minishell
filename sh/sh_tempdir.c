#include "shell.h"

static int	sh_cpy_tmpdir(char *dst, const char *src, size_t max_name_size);

static int	sh_build_tmp_path(char *dst, const char *tempdir,
				const char *prefix, const char *suffix);
static int	sh_build_tmp_suff_path(unsigned int n, char *path,
				char tempdir[SH_TMPDIR_MAX], const char *prefix);

int	sh_get_tmpdir(const t_env_store *env, char *tmpdir, size_t max_name_size)
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

int	sh_mktmpfd(const t_env_store *env, char *path, const char *prefix)
{
	char			tempdir[SH_TMPDIR_MAX];
	unsigned int	n;
	int				fd;

	if (sh_get_tmpdir(env, tempdir, SH_TMPDIR_MAX) != 0)
		return (-1);
	n = 0;
	while (n < 10000)
	{
		if (sh_build_tmp_suff_path(n, path, tempdir, prefix) != 0)
			return (-1);
		fd = open(path, O_CREAT | O_EXCL | O_WRONLY | SH_O_CLOEXEC,
				S_IRUSR | S_IWUSR);
		if (fd >= 0)
			return (fd);
		if (errno != EEXIST && errno != EINTR)
			return (-1);
		n++;
	}
	errno = EEXIST;
	return (-1);
}

static int	sh_cpy_tmpdir(char *dst, const char *src, size_t max_name_size)
{
	size_t	len;

	len = ft_strlen(src);
	while (len > 1 && src[len - 1] == '/')
		len--;
	if (len + 1 > max_name_size)
	{
		errno = ENAMETOOLONG;
		return (-1);
	}
	ft_memcpy(dst, src, len);
	dst[len] = '\0';
	return (0);
}

static int	sh_build_tmp_path(char *dst, const char *tempdir,
		const char *prefix, const char *suffix)
{
	const size_t	tempdir_len = ft_strlen(tempdir);
	const size_t	prefix_len = ft_strlen(prefix);
	const size_t	suffix_len = ft_strlen(suffix);
	size_t			counter;

	if (tempdir_len + 1 + prefix_len + 1 + suffix_len + 1 > SH_TMPPATH_MAX)
	{
		errno = ENAMETOOLONG;
		return (-1);
	}
	ft_memcpy(dst, tempdir, tempdir_len);
	counter = tempdir_len;
	dst[counter++] = '/';
	ft_memcpy(dst + counter, prefix, prefix_len);
	counter += prefix_len;
	dst[counter++] = '_';
	ft_memcpy(dst + counter, suffix, suffix_len);
	dst[counter + suffix_len] = '\0';
	return (0);
}

static int	sh_build_tmp_suff_path(unsigned int n, char *path,
		char tempdir[SH_TMPDIR_MAX], const char *prefix)
{
	char	*suff;

	suff = ft_utoa(n);
	if (!suff)
	{
		errno = ENOMEM;
		return (-1);
	}
	if (sh_build_tmp_path(path, tempdir, prefix, suff) != 0)
	{
		free(suff);
		return (-1);
	}
	free(suff);
	return (0);
}
