CC = cc
CFLAGS = -Werror -Wall -Wextra -MMD -MP
LIBFT_REPO = https://github.com/put/libft.git
LIBFT_COMMIT = e99f0e6

SERVER_SRCS = src/server/servermain.c src/server/serverhelpers.c src/shared.c
CLIENT_SRCS = src/client/clientmain.c src/client/clienthelpers.c src/shared.c
SERVER_OBJS = $(SERVER_SRCS:src/%.c=obj/%.o)
CLIENT_OBJS = $(CLIENT_SRCS:src/%.c=obj/%.o)
DEPS = $(SERVER_OBJS:.o=.d) $(CLIENT_OBJS:.o=.d)

all: libft server client

-include $(DEPS)

obj/server obj/client:
	@if [ ! -d "$@" ]; then \
		mkdir -p $@; \
	fi

libft:
	@if [ ! -d "libft" ]; then \
		git clone $(LIBFT_REPO) libft && \
		cd libft && git checkout $(LIBFT_COMMIT); \
	fi
	@if [ ! -f "libft/libft.a" ]; then \
		cd libft && make all; \
	fi

obj/%.o: src/%.c | obj/server obj/client
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

server: $(SERVER_OBJS) libft/libft.a
	$(CC) $(SERVER_OBJS) -L./libft -lft $(CFLAGS) -o server

client: $(CLIENT_OBJS) libft/libft.a
	$(CC) $(CLIENT_OBJS) -L./libft -lft $(CFLAGS) -o client

clean:
	rm -rf obj
	cd libft && make clean

fclean: clean
	rm -f server client
	rm -rf libft

re: fclean all

.PHONY: all clean fclean re libft obj/server obj/client