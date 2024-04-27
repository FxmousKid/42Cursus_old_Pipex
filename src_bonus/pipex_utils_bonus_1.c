/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inazaria <inazaria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 01:55:42 by inazaria          #+#    #+#             */
/*   Updated: 2024/04/27 20:14:48 by inazaria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	ft_err(char *str)
{
	ft_putstr_fd(str, 2);
}

/* Name : make_t_pipex
 *
 * Params : int argc -> the number of total arguemnents, taken from main.
 *			char *argv[] -> the arguments of the program, taken from main, 
 *							something in the form of {intfile, cmd1, cmd2, 
 *							outfile, NULL}
 *			char *env[] -> the environnement variables, taken from main *
 * 
 * Goal : Make the structure that i will be using to store : commands, pipes
 *        command counter, environnement variables, command counts.
 * 
 * Return : t_pipex * --  on success 
 *			NULL	  -- on failure			
 * */

t_pipex	*make_t_pipex_n_args(int argc, char *argv[], char *env[])
{
	t_pipex	*s_pipex;

	s_pipex = ft_calloc(1, sizeof(t_pipex));
	if (s_pipex == NULL)
		return (NULL);
	s_pipex->env = env;
	s_pipex->cmds = argv + 2;
	s_pipex->cmds_count = argc - 3;
	s_pipex->cmds_counter = 0;
	s_pipex->status = -1;
	s_pipex->cpid1 = -1;
	s_pipex->pipe_fds1 = ft_calloc(2, sizeof(int));
	s_pipex->pipe_fds2 = ft_calloc(2, sizeof(int));
	if (s_pipex->pipe_fds1 == NULL || s_pipex->pipe_fds2 == NULL)
		return (ft_err("calloc failed to make pipe fds\n"), NULL);
	if (pipe(s_pipex->pipe_fds2) < 0)
		return (perror("piping pipefd2 failed"), NULL);
	s_pipex->infile_fd = open(argv[1], O_RDONLY);
	if (s_pipex->infile_fd < 0)
		return (perror("open infile failed"), NULL);
	return (s_pipex);
}

t_pipex	*make_t_pipex_here_doc(int argc, char *argv[], char *env[])
{
	t_pipex	*s_pipex;

	s_pipex = ft_calloc(1, sizeof(t_pipex));
	if (s_pipex == NULL)
		return (NULL);
	s_pipex->env = env;
	s_pipex->cmds = argv + 2;
	s_pipex->cmds_count = argc - 4;
	s_pipex->cmds_counter = 2;
	s_pipex->status = -1;
	s_pipex->cpid1 = -1;
	s_pipex->pipe_fds1 = ft_calloc(2, sizeof(int));
	s_pipex->pipe_fds2 = ft_calloc(2, sizeof(int));
	if (s_pipex->pipe_fds1 == NULL || s_pipex->pipe_fds2 == NULL)
		return (ft_err("calloc failed to make pipe fds\n"), NULL);
	if (pipe(s_pipex->pipe_fds2) < 0)
		return (perror("piping pipefd2 failed"), NULL);
	s_pipex->infile_fd = -1;
	return (s_pipex);
}

int	clear_t_pipex(t_pipex *s_pipex)
{
	if (s_pipex->infile_fd > 0)
	{
		if (close(s_pipex->infile_fd) < 0)
			return (perror("closing infile_fd failed"), 1);
	}
	if (s_pipex->outfile_fd > 0)
	{
		if (close(s_pipex->outfile_fd) < 0)
			return (perror("closing outfile_fd failed"), 2);
	}
	free(s_pipex->pipe_fds1);
	free(s_pipex->pipe_fds2);
	free(s_pipex);
	return (0);
}

/* Name : find_path 
 * 
 * Params : char *env[] -> environnement variables taken from main
 *          char *cmd -> the string containing the command and its args
 *
 * Goal : Return a string that contains the full path to the executable
 *		  called, based on the PATHs present in environnement variables
 *
 * Return : a pointer to the path of the executable, if it exists in PATH
 *			NULL if the executable doesnt exist in PATH / or if a error
 *			happens at runtime. i.e. malloc failed, etc... / or if 
 *			permissions aren't good
 * */

char	*find_path(char *env[], char *cmd)
{
	char	**paths;
	char	*path_exec;
	char	*tmp;

	while (ft_strncmp(*env, "PATH=", 5))
		env++;
	paths = ft_split(*env, ':');
	if (paths == NULL)
		return (perror("split on PATH failed\n"), NULL);
	while (*paths)
	{
		tmp = ft_strjoin("/", cmd);
		if (tmp == NULL)
			return (perror("[pipex_utils -> find_path -> join1 |X|]\n"), NULL);
		path_exec = ft_strjoin(*paths, tmp);
		if (path_exec == NULL)
			return (perror("[pipex_utils -> find_path -> join2 |X|]\n"), NULL);
		if (access(path_exec, X_OK) == 0)
			return (path_exec);
		paths++;
	}
	return (perror(""), NULL);
}
