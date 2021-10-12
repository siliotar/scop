NAME= scop

SOURCE=	main.cpp

OSOURCEFOLDER= objects/

OSOURCE= $(addprefix $(OSOURCEFOLDER), $(SOURCE:.cpp=.o))

LIBS= -lGL -lglfw3 -lpthread -ldl -lm -lGLEW

all: $(NAME)

$(OSOURCEFOLDER):
	mkdir objects

$(OSOURCEFOLDER)%.o: %.c
	gcc -c $< -o $@

$(NAME): $(OSOURCEFOLDER) $(OSOURCE)
	gcc $(OSOURCE) -o $(NAME) $(LIBS)

clean:
	rm -rf $(OSOURCEFOLDER)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all
