/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inazaria <inazaria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 22:17:55 by inazaria          #+#    #+#             */
/*   Updated: 2024/04/27 00:24:43 by inazaria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

/* Name : child_processes
 *
 * Params : char *argv[] -> the arguments of the program taken from main, 
 *							something in the form of {intfile, cmd1, cmd2, 
 *							outfile, NULL}
 *			char *env[] -> the environnement variables taken fromdpi the main
 *
 * Goal : Execute command1 in the correct STDOUT_FILENO with the correct 
 *		  STDIN_FILENO this will only execute in the child, so we want to spawn
 *		  another child, that way we have one for each command, and we dont
 *		  have to worry about freeing
 *
 *	
 *						  |-->child1/child2   ==> execute cmd1 + redirections
 *						  |
 *	        ---> child1 --->child1/parent2  ==> execute cmd2 + redirections
 *			|
 *			|
 *	parent1 --------------->parent1		    ==> Handle failures
 *
 *  !!! Parent1 process is handled by pipex function, this only handles child1
 *  and its subprocesses !!!
 *
 *
 * Return : 0 on success, anything else indicates failure
 * */

int	child_processes(char *argv[], char *env[])
{
	pid_t	cpid2;
	int		fds[2];
	int		status;

	status = 0;
	if (pipe(fds) < 0)
		return (perror("[pipex.c -> child_processes -> failed piping]\n"), 1);
	cpid2 = fork();
	if (cpid2 < 0)
		return (perror("[pipex.c -> child_processes -> failed fork]\n"), 1);
	if (cpid2 == 0)
	{
		if (exec_command1(argv, env, fds) != 0)
			exit(0);
	}
	if (cpid2 > 0)
	{
		if (waitpid(cpid2, &status, 0) < 0)
			return (perror(""), 3);
		if (status == 0 && exec_command2(argv, env, fds) != 0)
			return (2);
		exit(0);
	}
	exit(0);
}

/* Name : pipex
 *
 *
 * Params : char *argv[] -> the arguments of the program taken from main, 
 *							something in the form of {intfile, cmd1, cmd2, 
 *							outfile, NULL}
 *			char *env[] -> the environnement variables taken fromdpi the main
 *
 * Goal : Replicate the "< infile cmd1 | cmd2 > outfile" command
 *		  spawn a child, make him do the execution of both commands by invoking 
 *		  another child then return the output
 *
 * Return : 0 for success, anything other indicates a error. i.e. failed malloc, 
 *			failed closing, failed opening, wrong permissions, ...
 * */

int	pipex(char *argv[], char *env[])
{
	pid_t	cpid1;
	int		status2;	

	status2 = 0;
	cpid1 = fork();
	if (cpid1 < 0)
		return (perror("[pipex.c -> pipex -> failed initial forking]\n"), 2);
	if (cpid1 == 0)
	{
		if (child_processes(argv, env) != 0)
			return (1);
	}
	if (cpid1 > 0)
	{
		if (waitpid(cpid1, &status2, 0) < 0)
			return (perror(""), 3);
		if (status2 != 0)
			return (1);
	}
	return (0);
}

int	main(int argc, char *argv[], char *env[])
{
	int	res;

	if (argc != 5)
		return (ft_err_1("Invalid arg count\n"), 1);
	res = pipex(argv, env);
	if (res != 0)
		return (1);
	return (0);
}
