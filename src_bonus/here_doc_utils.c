/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inazaria <inazaria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 13:48:10 by inazaria          #+#    #+#             */
/*   Updated: 2024/04/27 20:38:22 by inazaria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

int	exec_in_pipe_here_doc(t_pipex *s_pipex)
{
	char	*limiter;
	char	*line;
	int		write_qty;

	limiter = s_pipex->cmds[0];
	ft_printf(">");
	line = get_next_line(STDIN_FILENO);
	while (line != NULL && ft_strncmp(line, limiter, ft_strlen(limiter)) != 0)
	{
		write_qty = write(s_pipex->pipe_fds2[1], line, ft_strlen(line));
		if (write_qty < 0)
			return (perror("write from here_doc to pipe2[1] failed"), 1);
		if ((size_t) write_qty != ft_strlen(line))
			return (ft_err("partial write for line : "), ft_err(line), 2);
		ft_printf(">");
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	if (close(s_pipex->pipe_fds2[1]) < 0)
		return (perror("close pipe2[1] here_doc failed"), 4);
	return (0);
}
