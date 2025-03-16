NAME				= ircserv

CC 					= c++


SRCS 				= 	src/main.cpp \
						src/args_handler.cpp \
						src/Class/User/User.cpp \
						src/Class/Flag/Flag.cpp \
						src/Class/Canal/Canal.cpp \
						src/Class/Server/Server.cpp \
						src/Class/Server/Socket.cpp \
						src/Manager/commands/commandManager.cpp \
						src/Manager/userManager/UserManager.cpp \
						src/Manager/signals_manager.cpp \

OBJS_DIR			= .objs
OBJS				= ${SRCS:%.cpp=$(OBJS_DIR)/%.o}
DEPS				= ${OBJS:.o=.d}

CFLAGS				= -Wall -Wextra -Werror -std=c++98 -MMD -MP

$(OBJS_DIR)/%.o: %.cpp Makefile
			@mkdir -p $(dir $@)
			${CC} ${CFLAGS} -c $< -o $@

all:		${NAME}

${NAME}:	${OBJS}
			${CC} ${CFLAGS} ${OBJS} -o ${NAME}

-include ${DEPS}

clean:
			rm -rf ${OBJS_DIR}

fclean:		clean
			rm -f ${NAME}

re:			fclean all

.PHONY:		all clean fclean re


