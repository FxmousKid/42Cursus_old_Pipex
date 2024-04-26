/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inazaria <inazaria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:50:32 by inazaria          #+#    #+#             */
/*   Updated: 2024/04/26 22:37:56 by inazaria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


/* Name : launch_childs
 *
 * Params : t_pipex *s_pipex -> a pointer to the struct containing all infos
 *		    like the arguements, the environnement variables, the pipe fds...
 *
 * Goal : To loop on all the commands, and for each commands spawn a child,
 *		  Handle errors if any arise and return !0 on them, and continue 
 *		  for all the comands
 *
 * Return : 0     -- on success
 * 		    n > 0 -- on failure
 * */

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
	//	if (close(s_pipex->pipe_fds1[1]) < 0)
	//		return (perror("closing pipe_fds1[1] failed"), 4);
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


int	exec_in_pipe_outfile(t_pipex *s_pipex)
{
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


int	launch_childs(t_pipex *s_pipex)
{
	int	exec_infile;
	int	exec_n_pipe;
	int	exec_outfile;


	exec_infile = exec_in_pipe_infile(s_pipex);
	if (exec_infile != 0)
		return (ft_err("infile pipe failed\n"), exec_infile);
	while (s_pipex->cmds_counter < s_pipex->cmds_count - 1)
	{
		exec_n_pipe = exec_in_pipe(s_pipex);
		if (exec_n_pipe != 0)
			return (ft_err("n pipe failed\n"), exec_n_pipe);
	}
	exec_outfile = exec_in_pipe_outfile(s_pipex);
	if (exec_outfile != 0)
		return (ft_err("exec outfile failed\n"), exec_outfile);
	return (0);
}

/* Name : pipex_n_args
 *
 * Params : int argc -> the number of total arguemnents, taken from main.
 *			char *argv[] -> the arguments of the program, taken from main, 
 *							something in the form of {intfile, cmd1, cmd2, 
 *							outfile, NULL}
 *			char *env[] -> the environnement variables, taken from main *
 *
 * Goal : Initialize s_pipex, and call launch_childs to launch the child
 *		  processes. It handles child errors, and its own
 *			
 * Return : 0 -- on success
 *          n > 0 -- on failure
 * */

int	pipex_n_args(int argc, char *argv[], char *env[])
{
	t_pipex *s_pipex;

	s_pipex = make_t_pipex(argc, argv, env);
	if (s_pipex == NULL)
		return (ft_err("make_t_pipex failed\n"), 1);
	if (launch_childs(s_pipex) != 0)
		return (ft_err("launch_childs failed\n"), 2);
	if (clear_t_pipex(s_pipex) != 0)
		return (ft_err("clear_t_pipex failed\n"), 2);
	return (0);
}

int	main(int argc, char *argv[], char *env[])
{
	int	res;

	if (argc < 4)
		return (1);
	res = pipex_n_args(argc, argv, env);
	if (res != 0)
		return (res);
	return (0);
}
