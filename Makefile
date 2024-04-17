# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: inazaria <inazaria@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/17 20:24:39 by inazaria          #+#    #+#              #
#    Updated: 2024/04/17 20:51:19 by inazaria         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_FILE_TEST = $(wildcard ./src/test/*.c)

SRC_FILES = $(SRC_FILE_TEST)

OBJ_FILES = (SRC_FILES:.c=.o)


NAME        = pipex
CC          = clang
BUFFER_SIZE = 1024
INCLUDE     = ./includes/
CFLAGS      = -Wall -Wextra -Werror -g3 -D BUFFER_SIZE=$(BUFFER_SIZE) -I $(INCLUDE)


BLUE		= $(shell echo -e "\033[34m") 
YELLOW		= $(shell echo -e "\033[33m")
GREEN		= $(shell echo -e "\033[32m")
END			= $(shell echo -e "\033[0m")


.c.o :
	@$(CC) $(CFLAGS) -c $< -O $@

all : $(NAME)

$(NAME) : libft $(OBJ_FILES)
	@echo "$(YELLOW)Compiling pipex...$(END)"
	@$(CC) $(CFLAGS) $(OBJ_FILES) src/pipex.c -o $(NAME) libft/libft.a
	@echo "$(GREEN)Compiled pipex !$(END)"

libft : 
	@$(MAKE) -C ./libft


fclean : 
	@echo "$(YELLOW)Deleting pipex obj files...$(END)"
	@$(RM) $(OBJ_FILES) src/pipex.o
	@echo "$(YELLOW)Deleting pipex...$(END)"
	@$(RM) $(NAME) 
	@echo "$(YELLOW)Deleting libft obj files...$(END)"
	@echo "$(YELLOW)Deleting libft.a...$(END)"
	@$(MAKE) --no-print-directory -C ./libft fclean
	@echo "$(GREEN)Deleted all files !$(END)"

clean :
	@echo "$(YELLOW)Deleting pipex obj files...$(END)"
	@$(RM) $(OBJ_FILES)
	@echo "$(YELLOW)Deleting libft obj files...$(END)"
	@$(MAKE) --no-print-directory -C ./libft clean
	@echo "$(GREEN)Deleted all obj files !$(END)"

re : fclean all

.PHONY: all clean fclean re libft
