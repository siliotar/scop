NAME= scop

UNAME_S := $(shell uname -s)

SRCFOLDER= src/

SOURCE=	$(addprefix $(SRCFOLDER), \
		camera.c \
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
		utils.c \
		UI.c \
		matrix/matrix3.c \
		objChooser.c) \
		libs/glad/src/glad.c

OSOURCEFOLDER= .obj/

OSOURCE= $(addprefix $(OSOURCEFOLDER), $(SOURCE:.c=.o))

LIBS= -L ./libs/glfw-3.3.5/src/ -lglfw3 -lpthread -ldl -lm -L ./libs/
INCLUDES= -I include -I libs/glfw-3.3.5/include -I libs/glad/include

FLAGS= -Wall -Werror -Wextra

all: $(NAME)

$(OSOURCEFOLDER)%.o: %.c
	mkdir -p $(dir $@)
	gcc $(FLAGS) -c $< -o $@ -I /Users/rmass/.brew/include $(INCLUDES)

ifeq ($(UNAME_S),Darwin)
$(NAME): $(OSOURCE)
	gcc $(OSOURCE) -o $(NAME) -framework Cocoa -framework OpenGL -framework QuartzCore -framework IOKit $(LIBS)
else
$(NAME): $(OSOURCE)
	gcc $(OSOURCE) -o $(NAME) $(LIBS)
endif

clean:
	rm -rf $(OSOURCEFOLDER)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all
