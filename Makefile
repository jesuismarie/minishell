MAKEFLAGS		+= --no-print-directory

NAME			= minishell

BUILD			= ./build

SRC				= ./sources

DIRS			= built-in history parse \
				execute signal scanner utils

SRCSDIRS		= $(foreach dir, $(DIRS), $(addprefix $(SRC)/, $(dir))) $(SRC)

OBJS			= $(foreach dir, $(SRCSDIRS), \
				$(patsubst $(dir)/%.c, $(BUILD)/%.o, \
				$(shell find $(dir) -maxdepth 1 -name '*.c')))

INCS			= -I./includes -I./Libft/includes -I./readline_ahovakim_mnazarya/include

HEADER			= ./includes/minishell.h ./includes/define.h ./includes/get_next_line.h

FLAGS			= -Wall -Wextra -Werror -g3 -fsanitize=address -g

LIB				= -LLibft -lft -Lreadline_ahovakim_mnazarya/lib -lreadline

CC				= cc

ifeq ($(shell uname -s), Linux)
	LIB = -LLibft -lft -lreadline
endif

RESET			= \033[0m
BLUE			= \033[34m
YELLOW			= \033[38;2;255;239;0m
APPLE_GREEN		= \033[38;2;141;182;0m

$(BUILD)/%.o:	$(SRC)/*/%.c $(HEADER) Makefile
			@echo "${APPLE_GREEN} Compiling $<${RESET}"
			@$(CC) $(FLAGS) $(INCS) -c $< -o $@

$(BUILD)/%.o:	$(SRC)/%.c $(HEADER) Makefile
			@echo "${APPLE_GREEN} Compiling $<${RESET}"
			@$(CC) $(FLAGS) $(INCS) -c $< -o $@

all:			lib $(BUILD) $(NAME)

${NAME}:		${OBJS}
			@$(CC) $(FLAGS) $(OBJS) $(INCS) -o ${NAME} $(LIB)
			@echo "${YELLOW}Minishell created 🥑 ${RESET}"

$(BUILD):
			@mkdir -p $(BUILD)

configure:
			mkdir -p readline_ahovakim_mnazarya
			./readline_config.sh readline_ahovakim_mnazarya

lib:
			@make -C Libft

clean:
			@echo "${BLUE} Removing MiniShell $(BUILD)${RESET}"
			@make clean -C Libft
			@rm -rf $(BUILD)

fclean:
			@echo "${BLUE} Removing MiniShell $(BUILD)${RESET}"
			@echo "${BLUE} Removing ${NAME}${RESET}"
			@make fclean -C Libft
			@rm -rf $(BUILD)
			@rm -f ${NAME}
			@echo "${YELLOW} Everything is cleared ✅${RESET}"

re:			fclean all

.PHONY:		all clean fclean re lib configure
