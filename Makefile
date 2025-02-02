NAME := pipex
CC := cc
CPPFLAGS := -I./includes -Ilibft
CFLAGS := -Wall -Werror -Wextra
SRC := pipex.c
OBJDIR := ./obj
OBJ := $(SRC:%.c=$(OBJDIR)/%.o)
LDFLAGS := -Llibft
LDLIBS := -lft
LIBFTDIR := ./libft
LIBFT := $(LIBFTDIR)/libft.a

vpath %.c ./src

ifeq ($(DEBUG), true)
CFLAGS += -g -O0 -fsanitize=address
endif

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFTDIR)

$(warning obj=$(OBJ))

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJDIR)/%.o:%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $< -c -o $@ $(CPPFLAGS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	$(RM) -r $(NAME) $(OBJ) $(OBJDIR)
	$(MAKE) -C $(LIBFTDIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFTDIR) fclean

re: fclean
	$(MAKE) all

rr :
	make re && make clean

debug: clean
	$(MAKE) DEBUG=true

.PHONY: all clean fclean re rr debug bonus