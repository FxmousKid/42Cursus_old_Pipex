/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inazaria <inazaria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:15:07 by inazaria          #+#    #+#             */
/*   Updated: 2024/04/27 00:25:12 by inazaria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	ft_err_1(char *str)
{
	ft_putstr_fd(str, 2);
}

/* Name : exec_command2
 *
 * Params : char *argv[] -> the arguments of the program taken from main, 
 *							something in the form of {intfile, cmd1, cmd2, 
 *							outfile, NULL}
 *			char *env[] -> the environnement variables taken fromdpi the main *
 *			int	 fds[2] -> both end of the pipe
 *
 * Goal : Execute the 2nd command of pipex, use the read-end of the pipe as
 *		  STDIN and the outfile (argv[3]) as STDOUT
 *
 * Return : 0 on success, any other value on failure
 *
 * Notes : even though this function will be called in a child process, i'll 
 *		   still make it close all fds (the pipe ones are mandatory, but im
 *		   talking about the outfile one)
 * */

int	exec_command2(char *argv[], char *env[], int fds[2])
{
	char	*path;
	char	**cmd;
	char	outfile_fd;

	cmd = ft_split(argv[3], ' ');
	if (cmd == NULL)
		return (ft_err_1("[pipex_utils -> exec_command2 -> cmd_split\n]"), 1);
	path = find_path_1(env, cmd[0]);
	if (path == NULL)
		return (2);
	outfile_fd = open(argv[4], O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (outfile_fd < 0)
		return (perror("[pipex_utils -> exec_command2 -> open outfile]\n"), 3);
	if (dup2(outfile_fd, STDOUT_FILENO) < 0)
		return (perror("[pipex_utils -> exec_command2 -> dup2 stdout]\n"), 4);
	if (close(fds[1]) < 0)
		return (perror("[pipex_utils -> exec_command2 -> close write]\n"), 5);
	if (dup2(fds[0], STDIN_FILENO) < 0)
		return (perror("[pipex_utils -> exec_command2 -> dup2 stdin]\n"), 6);
	if (close(fds[0]) < 0)
		return (perror("[pipex_utils -> exec_command2 -> close read]\n"), 8);
	if (execve(path, cmd, env) < 0)
		return (perror("[pipex_utils -> exec_command2 -> exec failed]\n"), 7);
	exit(0);
}

/* Name : exec_command1
 *
 * Params : char *argv[] -> the arguments of the program taken from main, 
 *							something in the form of {intfile, cmd1, cmd2, 
 *							outfile, NULL}
 *			char *env[] -> the environnement variables taken fromdpi the main 
 *			int	 fds[2] -> both end of the pipe
 *
 * Goal : Execute the 1st command of pipex, use the infile (argv[0]) as 
 *        STDIN and the write-end of the pipe as STDOUT
 *
 * Return : 0 on success, any other value on failure
 *		   about the infile one)
 *
 * Notes : even though this function will be called in a child process, i'll 
 *		   still make it close all fds (the pipe ones are mandatory, but im
 *		   talking about the outfile one)
 * */

int	exec_command1(char *argv[], char *env[], int fds[2])
{
	char	*path;
	char	**cmd;
	int		infile_fd;

	cmd = ft_split(argv[2], ' ');
	if (cmd == NULL)
		return (ft_err_1("[pipex_utils -> exec_command1 -> cmd_split]\n"), 1);
	path = find_path_1(env, cmd[0]);
	if (path == NULL)
		return (2);
	infile_fd = open(argv[1], O_RDONLY);
	if (infile_fd < 0)
		return (perror("[pipex_utils -> exec_command1 -> open infile\n]"), 3);
	if (dup2(infile_fd, STDIN_FILENO) < 0)
		return (perror("[pipex_utils -> exec_command1 -> dup2 stdin\n]"), 4);
	if (close(fds[0]) < 0)
		return (perror("[pipex_utils -> exec_command1 -> close read\n]"), 5);
	if (dup2(fds[1], STDOUT_FILENO) < 0)
		return (perror("[pipex_utils -> exec_command1 -> dup2 stdout\n]"), 6);
	if (close(fds[1]) < 0)
		return (perror("[pipex_utils -> exec_command1 -> close write\n]"), 8);
	if (execve(path, cmd, env) < 0)
		return (perror("[pipex_utils -> exec_command1 -> exec failed\n]"), 7);
	exit(1);
}

/* Name : find_path_1
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

char	*find_path_1(char *env[], char *cmd)
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
