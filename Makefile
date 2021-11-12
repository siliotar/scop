NAME= scop

SOURCE=	src/camera.c \
		src/events.c \
		src/fileReader.c \
		src/main.c \
		src/mesh.c \
		src/objParser.c \
		src/renderer.c \
		src/scop.c \
		src/Shaders.c \
		src/stb_image.c \
		src/texture.c \
		src/matrix/matrix4.c \
		src/vector/vector3.c \
		src/vector/vector4.c

OSOURCEFOLDER= objects/

OSOURCE= $(addprefix $(OSOURCEFOLDER), $(SOURCE:.c=.o))

LIBS= -lGL -lglfw3 -lpthread -ldl -lm -lGLEW

FLAGS= -Wall -Werror -Wextra

all: $(NAME)

$(OSOURCEFOLDER):
	mkdir objects
	mkdir objects/src
	mkdir objects/src/matrix
	mkdir objects/src/vector

$(OSOURCEFOLDER)%.o: %.c
	gcc $(FLAGS) -c $< -o $@

$(NAME): $(OSOURCEFOLDER) $(OSOURCE)
	gcc $(OSOURCE) -o $(NAME) $(LIBS)

clean:
	rm -rf $(OSOURCEFOLDER)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all
