/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exec_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inazaria <inazaria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 02:06:09 by inazaria          #+#    #+#             */
/*   Updated: 2024/04/28 01:31:00 by inazaria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

int	exec(t_pipex *s_pipex)
{
	char	**cmd_w_opts;
	char	*path_to_cmd;

	if (s_pipex->cpid1 > 0)
		return (ft_err("exec called in parent process\n"), 1);
	cmd_w_opts = ft_split((s_pipex->cmds)[s_pipex->cmds_counter - 1], ' ');
	if (cmd_w_opts == NULL)
		return (ft_err("splitting cmd_w_opts failed \n"), 1);
	path_to_cmd = find_path(s_pipex->env, cmd_w_opts[0]);
	if (path_to_cmd == NULL)
		return (ft_err("Error : command not found\n"), 2);
	if (execve(path_to_cmd, cmd_w_opts, s_pipex->env) != 0)
		return (perror("exec failed"), 3);
	return (4);
}

int	exec_in_pipe_infile(t_pipex *s_pipex)
{
	s_pipex->cmds_counter += 1;
	s_pipex->cpid1 = fork();
	if (s_pipex->cpid1 < 0)
		return (perror("forking 1 failed"), 2);
	if (s_pipex->cpid1 == 0)
	{
		if (redirect(s_pipex->infile_fd, s_pipex->pipe_fds2[1]) != 0)
			return (ft_err("redirection 1 failed"), 3);
		if (exec(s_pipex) != 0)
			return (ft_err("exec failed\n"), 4);
	}
	if (s_pipex->cpid1 > 0)
	{
		if (waitpid(s_pipex->cpid1, &s_pipex->status, 0) < 0)
			return (perror("waitpid failed"), 5);
		if (close(s_pipex->pipe_fds2[1]) < 0)
			return (perror("closing pipe_fds2[1] failed"), 5);
		if (s_pipex->status != 0)
			return (ft_err("child 1 failed\n"), 6);
	}
	return (0);
}

int	exec_in_pipe(t_pipex *s_pipex)
{
	s_pipex->cmds_counter += 1;
	ft_memcpy(s_pipex->pipe_fds1, s_pipex->pipe_fds2, sizeof(int) * 2);
	if (make_new_pipe(s_pipex) != 0)
		return (ft_err("make_new_pipe failed\n"), 1);
	s_pipex->cpid1 = fork();
	if (s_pipex->cpid1 < 0)
		return (perror("forking 2 failed"), 2);
	if (s_pipex->cpid1 == 0)
	{
		if (redirect(s_pipex->pipe_fds1[0], s_pipex->pipe_fds2[1]) != 0)
			return (ft_err("redirection 2 failed\n"), 3);
		if (exec(s_pipex) != 0)
			return (ft_err("exec failed\n"), 4);
	}
	if (s_pipex->cpid1 > 0)
	{
		if (waitpid(-1, &s_pipex->status, 0) < 0)
			return (perror("waitpid failed"), 4);
		if (close(s_pipex->pipe_fds2[1]) < 0)
			return (perror("closing pipe_fds2[1] failed"), 4);
		if (s_pipex->status != 0)
			return (ft_err("child 2 failed\n"), 5);
	}
	return (0);
}

int	exec_in_pipe_outfile_n_args(t_pipex *s_pipex)
{
	open_outfile_n_args(s_pipex);
	if (s_pipex->outfile_fd < 0)
		return (ft_err("open_outfile_n_args failed\n"), s_pipex->outfile_fd);
	s_pipex->cmds_counter += 1;
	s_pipex->cpid1 = fork();
	if (s_pipex->cpid1 < 0)
		return (perror("forking 3 failed or closing pipe_fds1[1] failed"), 2);
	if (s_pipex->cpid1 == 0)
	{
		if (redirect(s_pipex->pipe_fds2[0], s_pipex->outfile_fd) != 0)
			return (ft_err("redirection 3 failed"), 3);
		if (exec(s_pipex) != 0)
			return (ft_err("exec failed\n"), 4);
	}
	if (s_pipex->cpid1 > 0)
	{
		if (close(s_pipex->pipe_fds1[0]) != 0)
			return (perror("closing read-end pipe failed"), 4);
		if (waitpid(-1, &s_pipex->status, 0) < 0)
			return (perror("waitpid failed"), 5);
		if (s_pipex->status != 0)
			return (ft_err("child 3 failed\n"), 6);
	}
	return (0);
}

int	exec_in_pipe_outfile_here_doc(t_pipex *s_pipex)
{
	open_outfile_here_doc(s_pipex);
	if (s_pipex->outfile_fd < 0)
		return (ft_err("open_outfile_here_doc failed\n"), s_pipex->outfile_fd);
	if (s_pipex->outfile_fd < 0)
		return (perror("open outfile failed"), 1);
	s_pipex->cpid1 = fork();
	if (s_pipex->cpid1 < 0)
		return (perror("forking 3 failed or closing pipe_fds1[1] failed"), 2);
	if (s_pipex->cpid1 == 0)
	{
		if (redirect(s_pipex->pipe_fds2[0], s_pipex->outfile_fd) != 0)
			return (ft_err("redirection 3 failed"), 3);
		if (exec(s_pipex) != 0)
			return (ft_err("exec failed\n"), 4);
	}
	if (s_pipex->cpid1 > 0)
	{
		if (close(s_pipex->pipe_fds1[0]) != 0)
			return (perror("closing read-end pipe failed"), 4);
		if (waitpid(-1, &s_pipex->status, 0) < 0)
			return (perror("waitpid failed"), 5);
		if (s_pipex->status != 0)
			return (ft_err("child 3 failed\n"), 6);
	}
	return (0);
}
