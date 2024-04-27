/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inazaria <inazaria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 20:52:42 by inazaria          #+#    #+#             */
/*   Updated: 2024/04/27 00:25:24 by inazaria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <errno.h>

# include "../libft/include/libft.h"

int		pipe_test(void);
int		pipe_2way(void);
int		exec_redirect(char *argv[], char *env[]);

int		pipex(char *argv[], char *env[]);
int		child_processes(char *argv[], char *env[]);
int		exec_command1(char *argv[], char *env[], int fds[2]);
int		exec_command2(char *argv[], char *env[], int fds[2]);
char	*find_path_1(char *env[], char *cmd);
void	ft_err_1(char *str);

#endif
