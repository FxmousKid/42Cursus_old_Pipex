# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: inazaria <inazaria@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/17 20:24:39 by inazaria          #+#    #+#              #
#    Updated: 2024/04/27 14:08:24 by inazaria         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_FILES_STANDARD := $(wildcard ./src/*.c)
SRC_FILES_BONUS    := $(wildcard ./src_bonus/*.c)

OBJ_FILES_STANDARD := $(SRC_FILES_STANDARD:.c=.o)
OBJ_FILES_BONUS    := $(SRC_FILES_BONUS:.c=.o)

NAME        := pipex
NAME_BONUS  := pipex_bonus
CC          := clang
INCLUDE     := ./includes/
CFLAGS      := -Wall -Wextra -Werror -g3 -I $(INCLUDE)
RM          := rm -f

BLUE		:= $(shell echo -e "\033[34m") 
YELLOW		:= $(shell echo -e "\033[33m")
GREEN		:= $(shell echo -e "\033[32m")
END			:= $(shell echo -e "\033[0m")


all : $(NAME)


$(NAME) : libft $(OBJ_FILES_STANDARD)
	@echo "$(YELLOW)Compiling pipex...$(END)"
	@$(CC) $(CFLAGS) $(OBJ_FILES_STANDARD) -o $(NAME) ./libft/libft.a 
	@echo "$(GREEN)Compiled pipex !$(END)"

bonus : libft $(OBJ_FILES_BONUS)
	@echo "$(YELLOW)Compiling pipex_bonus...$(END)"
	@$(CC) $(CFLAGS) $(OBJ_FILES_BONUS) -o $(NAME_BONUS) ./libft/libft.a
	@echo "$(GREEN)Compiled pipex_bonus !$(END)"


libft :
	@$(MAKE) --no-print-directory -C ./libft all

fclean :
	@echo "$(YELLOW)Deleting pipex obj files...$(END)"
	@$(RM) $(OBJ_FILES) 
	@echo "$(YELLOW)Deleting pipex_bonus obj files...$(END)"
	@$(RM) $(OBJ_FILES_BONUS)
	@echo "$(YELLOW)Deleting pipex...$(END)"
	@$(RM) $(NAME) 
	@echo "$(YELLOW)Deleting pipex_bonus...$(END)"
	@$(RM) $(NAME_BONUS)
	@echo "$(YELLOW)Deleting libft obj files...$(END)"
	@echo "$(YELLOW)Deleting libft.a...$(END)"
	@$(MAKE) --no-print-directory -C ./libft fclean
	@echo "$(GREEN)Deleted all files !$(END)"

clean :
	@echo "$(YELLOW)Deleting pipex obj files...$(END)"
	@$(RM) $(OBJ_FILES) 
	@echo "$(YELLOW)Deleting pipex_bonus obj files...$(END)"
	@$(RM) $(OBJ_FILES_BONUS)
	@echo "$(YELLOW)Deleting libft obj files...$(END)"
	@$(MAKE) --no-print-directory -C ./libft clean
	@echo "$(GREEN)Deleted all obj files !$(END)"

re : fclean all

.PHONY: all clean fclean re libft
