/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inazaria <inazaria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:51:25 by inazaria          #+#    #+#             */
/*   Updated: 2024/06/03 20:44:29 by inazaria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

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

typedef struct s_pipex
{
	char	**env;
	char	**cmds;
	int		cmds_count;
	int		cmds_counter;
	int		*pipe_fds1;
	int		*pipe_fds2;
	int		infile_fd;
	int		outfile_fd;
	int		status;
	pid_t	cpid1;
}				t_pipex;

void	ft_err(char *str);
char	*find_path(char *env[], char *cmd);
t_pipex	*make_t_pipex_n_args(int argc, char *argv[], char *env[]);
t_pipex	*make_t_pipex_here_doc(int argc, char *argv[], char *env[]);
int		clear_t_pipex(t_pipex *s_pipex);
int		launch_childs(t_pipex *s_pipex);
int		exec(t_pipex *s_pipex);
int		exec_in_pipe_here_doc(t_pipex *s_pipex);
int		exec_in_pipe_infile(t_pipex *s_pipex);
int		exec_in_pipe(t_pipex *s_pipex);
int		exec_in_pipe_outfile_n_args(t_pipex *s_pipex);
int		exec_in_pipe_outfile_here_doc(t_pipex *s_pipex);
void	open_outfile_n_args(t_pipex *s_pipex);
void	open_outfile_here_doc(t_pipex *s_pipex);
int		make_new_pipe(t_pipex *s_pipex);

int		redirect(int new_stdin, int new_stdout);
int		pipex(int argc, char *argv[], char *env[]);

#endif
