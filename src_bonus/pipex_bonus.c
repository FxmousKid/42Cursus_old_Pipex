/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inazaria <inazaria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:50:32 by inazaria          #+#    #+#             */
/*   Updated: 2024/04/27 02:05:34 by inazaria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

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
	t_pipex	*s_pipex;

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
