#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# define FD_MAX 4096
# define BUFFER_SIZE 1024

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>

char *get_next_line(int fd);
char *ft_strchr(const char *s, int c);
char *ft_strjoin(char *s1, char *s2);
char *ft_strdup(const char *s1);
size_t ft_strlen(const char *s);
#endif
