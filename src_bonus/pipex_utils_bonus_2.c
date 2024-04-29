/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inazaria <inazaria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 02:00:11 by inazaria          #+#    #+#             */
/*   Updated: 2024/04/28 01:30:59 by inazaria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

int	redirect(int new_stdin, int new_stdout)
{
	if (new_stdin > 0 && dup2(new_stdin, STDIN_FILENO) < 0)
		return (perror("dup2 failed STDIN"), 1);
	if (new_stdout > 0 && dup2(new_stdout, STDOUT_FILENO) < 0)
		return (perror("dup2 failed STDOUT"), 2);
	return (0);
}

int	make_new_pipe(t_pipex *s_pipex)
{
	free(s_pipex->pipe_fds2);
	s_pipex->pipe_fds2 = ft_calloc(sizeof(int), 2);
	if (s_pipex->pipe_fds2 == NULL)
		return (ft_err("calloc make_new_pipe failed"), 1);
	if (pipe(s_pipex->pipe_fds2) < 0)
		return (perror("piping new pipe failed"), 2);
	return (0);
}

void	open_outfile_n_args(t_pipex *s_pipex)
{
	char	*outfile;

	outfile = s_pipex->cmds[s_pipex->cmds_count];
	s_pipex->outfile_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (s_pipex->outfile_fd < 0)
		ft_err("open outfile failed\n");
	return ;
}

void	open_outfile_here_doc(t_pipex *s_pipex)
{
	char	*outfile;

	outfile = s_pipex->cmds[s_pipex->cmds_count + 1];
	s_pipex->outfile_fd = open(outfile, O_WRONLY | O_APPEND, 0644);
	if (s_pipex->outfile_fd < 0)
		perror("open outfile failed");
	return ;
}
