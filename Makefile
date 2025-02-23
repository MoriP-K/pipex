NAME := pipex
CC := cc
CPPFLAGS := -I./includes -I./src/libft
CFLAGS := -Wall -Werror -Wextra
SRC := pipex.c init.c free_any.c error.c do_func.c close_fd.c\
	read_and_write.c find_cmd_1.c find_cmd_2.c
OBJDIR := ./obj
OBJ := $(SRC:%.c=$(OBJDIR)/%.o)
LDFLAGS := -L./src/libft
LDLIBS := -lft
LIBFTDIR := ./src/libft
LIBFT := $(LIBFTDIR)/libft.a

vpath %.c ./src

ifeq ($(DEBUG), true)
CFLAGS += -g -O0 -fsanitize=address
CFLAGS := -Wall -Wextra 
endif

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFTDIR)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJDIR)/%.o:%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $< -c -o $@ $(CPPFLAGS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

# $(warning obj=$(OBJ))

clean:
	$(RM) -r $(OBJ) $(OBJDIR)
	$(MAKE) -C $(LIBFTDIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFTDIR) fclean

re: fclean
	$(MAKE) all

rr :
	make re && make clean

d: clean
	$(MAKE) DEBUG=true

.PHONY: all clean fclean re rr d bonus