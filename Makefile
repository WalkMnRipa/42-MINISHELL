# Colors
GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
BLUE = \033[0;34m
RESET = \033[0m

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline

SRCS = src/main.c src/init/init.c src/execution/pipeline_utils.c src/execution/env_utils.c \
	src/execution/pipeline.c src/execution/custom_setenv.c src/execution/cleanup.c \
	src/execution/redirection.c src/execution/env_utils1.c src/execution/signals.c \
	src/execution/heredoc.c src/execution/heredoc_utils.c src/execution/heredoc_utils2.c \
	src/execution/heredoc_utils3.c src/execution/heredoc_utils4.c src/execution/execute_command.c src/execution/pipeline_utils1.c \
	src/execution/utils.c src/execution/status_utils.c src/execution/redirection_utils.c \
	src/execution/execute_non_builtins.c src/execution/command_utils.c \
	src/execution/builtins/exit.c src/execution/builtins/export.c src/execution/builtins/cd.c \
	src/execution/builtins/pwd.c src/execution/builtins/echo.c src/execution/builtins/export_print.c \
	src/execution/builtins/builtins.c src/execution/builtins/export_utils.c \
	src/execution/builtins/update_pwd.c src/execution/builtins/env.c src/execution/builtins/unset.c \
	src/parsing/syntax_check.c src/parsing/variable_expansion_utils.c src/parsing/quote_handler_utils.c \
	src/parsing/command_parser_utils.c src/parsing/token_utils.c src/parsing/tokenizer.c \
	src/parsing/tokenizer_utils.c src/parsing/command_parser.c src/parsing/variable_expansion.c \
	src/parsing/quote_handler.c src/parsing/token_utils1.c src/parsing/command_parser_utils2.c \
	src/parsing/quote_handler_utils2.c src/execution/cleanup2.c

OBJS_DIR = objs
OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)

DEPS = $(OBJS:.o=.d)

LIBFT = libft/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(OBJS) -Llibft -lft $(LDFLAGS) -o $(NAME)
	@echo "$(BLUE)$(NAME) created!$(RESET)"

$(OBJS_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@printf "$(YELLOW)Compiling $<... $(RESET)"
	@if $(CC) $(CFLAGS) -MMD -MP -I./includes -Ilibft -c $< -o $@; then \
		printf "$(GREEN)Done!$(RESET)\n"; \
	else \
		printf "$(RED)Failed!$(RESET)\n"; \
		exit 1; \
	fi

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