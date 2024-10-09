# Colors
GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
BLUE = \033[0;34m
RESET = \033[0m

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = src/main.c src/parsing/cleanup.c src/parsing/tokenizer.c src/utils/parsing_utils/handles_token_utils.c src/execution/execution_cleanup.c src/execution/env_utils.c src/execution/execute_command.c src/execution/heredoc.c src/execution/pipeline.c src/execution/utils.c src/execution/builtins/echo.c src/execution/builtins/env.c src/execution/builtins/exit.c src/execution/builtins/export.c src/execution/builtins/pwd.c src/execution/builtins/unset.c src/execution/builtins/update_pwd.c src/execution/builtins/cd.c

OBJS_DIR = objs
OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)

DEPS = $(OBJS:.o=.d)

LIBFT = libft/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) -Llibft -lft -lreadline -o $(NAME)
	@echo "$(BLUE)$(NAME) created!$(RESET)"

$(OBJS_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@printf "$(YELLOW)Compiling $<... $(RESET)"
	@if $(CC) $(CFLAGS) -MMD -MP -I./includes -Ilibft -c $< -o $@; then 		printf "$(GREEN)Done!$(RESET)\n"; 	else 		printf "$(RED)Failed!$(RESET)\n"; 		exit 1; 	fi

$(LIBFT):
	@echo "$(YELLOW)Compiling libft...$(RESET)"
	@$(MAKE) -C libft > /dev/null 2>&1
	@echo "$(GREEN)libft compilation done!$(RESET)"

clean:
	@echo "$(YELLOW)Cleaning up...$(RESET)"
	@$(MAKE) -C libft clean > /dev/null
	@rm -rf $(OBJS_DIR)
	@echo "$(GREEN)Clean done!$(RESET)"

fclean: clean
	@echo "$(YELLOW)Full cleanup...$(RESET)"
	@$(MAKE) -C libft fclean > /dev/null
	@rm -f $(NAME)
	@echo "$(GREEN)Full cleanup done!$(RESET)"

re: fclean all

.PHONY: all clean fclean re

-include $(DEPS)
