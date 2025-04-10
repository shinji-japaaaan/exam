#include "get_next_line.h"

void safe_free(char **str)
{
    if (str != NULL || *str != NULL)
    {
        free(*str);
        *str = NULL; 
    }
}

char *ft_strchr_len(char *str, char c)
{
    int i;

    if (str == NULL)
        return (NULL);
    i = 0;
    while (str[i] != 0)
    {
        if (str[i] == c)
            return (str);
        i++;
    }
    return (NULL);
}

size_t	ft_strlen(char *str)
{
	size_t	i;

	i = 0;
	while (*str != '\0')
	{
		str++;
		i++;
	}
	return (i);
}

char *ft_strnjoin(char *str1, char *str2, int count_byte)
{
    int len;
    char *new_str;
    int i;
    int j;

    if (str1 == NULL || str2 == NULL || count_byte < 0)
        return (NULL);
    len = ft_strlen(str1);
    new_str = malloc(sizeof(char) * (len + count_byte + 1));
    if (new_str == NULL)
        return (NULL);
    i = 0;
    while (str1[i] != 0)
    {
        new_str[i] = str1[i];
        i++;
    }
    j = 0;
    while (j < count_byte && str2[j] != 0)
    {
        new_str[i] = str2[j];
        i++;
        j++;
    }
    new_str[i] = '\0';
    return (new_str);
}

static int append_buf(char **str, char *buf, int count_byte)
{
    char *new_str;
    char *temp;

    buf[count_byte] = '\0';
    temp = *str;
    *str = ft_strnjoin(*str, buf, count_byte);
    if (*str == NULL)
    {
        free(temp);
        return (-1);
    }
    free(temp);
    return (0);
}

int read_fd(int fd, char **str)
{
    char *buf;
    int count_byte;

    buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
    if (buf == NULL)
        return (-1);
    count_byte = 1;
    while (count_byte)
    {
        count_byte = read(fd, buf, BUFFER_SIZE);
        if (count_byte <= 0)
            break;
        if (append_buf(str, buf, count_byte) < 0)
            break;
        if (ft_strchr_len(*str, '\n'))
            break;
    }
    free(buf);
    if (count_byte < 0)
        return (-1);    
    return (count_byte);//0でなくていい？
}

char *ft_strncpy(char *line, char *str, int len)
{
    int i;

    if (line == NULL || str == NULL)
        return (NULL);
    i = 0;
    while (i < len && str[i] != 0)
    {
        line[i] = str[i];
        i++;
    }
    while (i < len)
    {
        line[i] = '\0';
        i++;
    }
    line[len] = '\0';
    return (line);
}

char *ft_strcpy(char *str, char *src)
{
    int i;

    if (str == NULL || src == NULL)
        return (NULL);
    i = 0;
    while (src[i] != 0)
    {
        str[i] = src[i];
        i++;
    }
    str[i] = '\0';
    return (str);
}

char *find_newline(char **str)
{
    char *line;
    char *new_position;
    int len;

    new_position = ft_strchr_len(*str, '\n');
    if (new_position)
        len = new_position - *str + 1;
    else
        len = ft_strlen(*str);
    line = malloc(sizeof(char) * (len + 1));
    if (line == NULL)
        return (NULL);
    ft_strncpy(line, *str, len);
    if (new_position)
        ft_strcpy(*str, new_position + 1);
    else
        safe_free(str);
    return (line);
}

char *ft_strdup(char *str)
{
    char *new_str;
    int len;
    int i;

    if (str == NULL)
        return (NULL);
    len = ft_strlen(str);
    new_str = malloc(sizeof(char) * (len + 1));
    if (new_str == NULL)
        return (NULL);
    i = 0;
    while (str[i] != 0)
    {
        new_str[i] = str[i];
        i++;
    }
    new_str[i] = '\0';
    return (new_str);
}

char	*get_next_line(int fd)
{
	static char	*str[FD_MAX];
	char		*line;
	int			size;
    int         res;

	if (fd < 0 || FD_MAX <= fd || BUFFER_SIZE <= 0)
		return (NULL);
    if (str[fd] == NULL)
        str[fd] = ft_strdup("");
    if (str[fd] == NULL)
        return(NULL);
	res = read_fd(fd, &str[fd]);
	if (res < 0 || *str[fd] == '\0')
    {
        safe_free(&str[fd]);
        return (NULL);
    }
    line = find_newline(&str[fd]);
    if (line == NULL)
    {
        safe_free(&str[fd]);
        return (NULL);
    }
	return (line);
}

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("text.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (1);
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
        line = NULL;
	}
    if (close(fd) == -1)
    {
        perror("failed");
        return (1);
    }
	return (0);
}

