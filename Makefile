NAME= scop

SOURCE=	camera.c \
		events.c \
		fileReader.c \
		main.c \
		mesh.c \
		objParser.c \
		renderer.c \
		scop.c \
		Shaders.c \
		texture.c \
		matrix/matrix4.c \
		vector/vector3.c \
		vector/vector4.c \
		bmp_reader.c \
		utils.c

OSOURCEFOLDER= .obj/

OVECTORFOLDER= .obj/vector/

OMATRIXFOLDER= .obj/matrix/

SRCFOLDER= src/

OSOURCE= $(addprefix $(OSOURCEFOLDER), $(SOURCE:.c=.o))

LIBS= -lGL -lglfw3 -lpthread -ldl -lm -lGLEW

FLAGS= -Wall -Werror -Wextra

all: $(NAME)

$(OSOURCEFOLDER): $(OVECTORFOLDER) $(OMATRIXFOLDER)

$(OVECTORFOLDER):
	mkdir -p $(OVECTORFOLDER)

$(OMATRIXFOLDER):
	mkdir -p $(OMATRIXFOLDER)

$(OSOURCEFOLDER)%.o: $(SRCFOLDER)%.c
	gcc $(FLAGS) -c $< -o $@

$(NAME): $(OSOURCEFOLDER) $(OSOURCE)
	gcc $(OSOURCE) -o $(NAME) $(LIBS)

clean:
	rm -rf $(OSOURCEFOLDER)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all
