#include "get_next_line_retry.h"

void safe_free(char **str)
{
    if (str != NULL && *str != NULL)
    {
        free(*str);
        *str = NULL;
    }
}

int ft_strlen(char *str)
{
    int len;

    len = 0;
    while (*str)
    {
        str++;
        len++;
    }
    return (len);
}

char *ft_strdup(char *str)
{
    int len;
    char *result;
    int i;

    if (str == NULL)
        return (NULL);
    len = ft_strlen(str);
    result = malloc(len + 1);
    if (result == NULL)
        return (NULL);
    i = 0;
    while (str[i])
    {
        result[i] = str[i];
        i++;
    }
    result[i] = '\0';
    return (result);
}

char *ft_strnjoin(char *str, char *buf, int n)
{
    int len;
    char *result;
    int i;
    int j;

    len = ft_strlen(str);
    result = malloc(len + n);
    if (result == NULL)
        return (NULL);
    i = 0;
    while (str[i])
    {
        result[i] = str[i];
        i++;
    }
    j = 0;
    while (j < n && buf[j])
    {
        result[i] = buf[j];
        i++;
        j++;
    }
    result[i] = '\0'; //ここが違う
    return (result);
}

int append_buf(char **str, char *buf, int n)
{
    char *tmp;

    buf[n] = '\0';
    tmp = *str;
    *str = ft_strnjoin(*str, buf, n);
    if (*str == NULL)
    {
        free(tmp);
        return (-1);
    }
    free(tmp);
    return (0);
}

char *ft_strchr_len(char *str, int c)
{
    int i;

    if (str == NULL)
        return (NULL);
    i = 0;
    while (*str)
    {
        if (*str == (char)c)
            return (str);
        str++;
    }
    return (NULL);
}

int read_fd(int fd, char **str)
{
    char *buf;
    int count_byte;
    
    if (str == NULL || *str == NULL)
        return (-1);
    buf = malloc(BUFFER_SIZE + 1);
    if (!buf)
        return (-1);
    count_byte = 1;
    while (count_byte)
    {
        if ((count_byte = read(fd, buf, BUFFER_SIZE)) <= 0)
            break;
        if (append_buf(str, buf, count_byte) < 0)
            break;
        if (ft_strchr_len(*str, '\n'))
            break;
    }
    free(buf);
    if (count_byte < 0)
        return (-1);
    return (0);
}

char *ft_strncpy(char *line, char *str, int len)
{
    int i;

    if (line == NULL || str == NULL | len < 0)
        return (NULL);
    i = 0;
    while (str[i] && i < len)
    {
        line[i] = str[i];
        i++;
    }
    while (i < len)
    {
        line[i] = '\0';
        i++;
    }
    line[i] = '\0';
    return (str);
}

char *ft_strcpy(char *str, char *str2)
{
    int i;

    if (str == NULL || str2 == NULL)
        return (NULL);
    i = 0;
    while (str2[i])
    {
        str[i] = str2[i];
        i++;
    }
    str[i] = '\0';
    return (str);
}

char *find_newline(char **str)
{
    char *line;
    char *new_pos;
    int len;

    new_pos = ft_strchr_len(*str, '\n');
    if (new_pos)
        len = new_pos - *str + 1;
    else
        len = ft_strlen(*str);
    line = malloc(len + 1);
    if (line == NULL)
    {
        return (NULL);
    }
    ft_strncpy(line, *str, len);
    if (new_pos)
        ft_strcpy(*str, new_pos + 1);
    else
        safe_free(str);
    return (line);
}

char *get_next_line(int fd)
{
    static char *str[FD_MAX];
    char *line;
    int res;

    if (fd < 0 || fd >= FD_MAX || 0 > FD_MAX)
        return (NULL);
    if (str[fd] == NULL)
        str[fd] = ft_strdup("");
    if (str[fd] == NULL)
        return (NULL);
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

int main()
{
    int fd;
    char *line;

    fd = open("text.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("failed");
        return (1);
    }
    while ((line = get_next_line(fd) ) != NULL)
    {
        printf("%s\n", line);
        free(line);
        line = NULL;
    }
    if (close(fd) == -1)
    {
        free(line);
        perror("failed");
        return (1);
    }
    return (0);
}
