NAME= scop

UNAME_S = $(shell uname -s)

SRCDIR = src/

SOURCE=	$(addprefix $(SRCDIR), \
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

OBJDIR = .obj/

DEPDIR = .dep/

OBJ = $(addprefix $(OBJDIR), $(SOURCE:.c=.o))

DEP = $(addprefix $(DEPDIR), $(SOURCE:.c=.d))

ifeq ($(UNAME_S),Darwin)
LIBS = -L ./libs/glfw-3.3.5/src/ -lglfw3 -lpthread -ldl -lm -L ./libs/
else
LIBS = -L ./libs/glfw-3.3.5/src/ -lglfw3 -lpthread -ldl -lm -lGL -lX11 -L ./libs/
endif

INCLUDES = -I include -I libs/glfw-3.3.5/include -I libs/glad/include

FLAGS = -Wall -Werror -Wextra

all: $(NAME)

$(DEPDIR)%.d: %.c
	mkdir -p $(dir $@)
	gcc -MT $(<:%.c=$(OBJDIR)%.o) -MM $< > $@ $(INCLUDES)

$(OBJDIR)%.o: %.c
	mkdir -p $(dir $@)
	gcc $(FLAGS) -c $< -o $@ $(INCLUDES)

$(NAME): $(DEP) $(OBJ)
	cd libs/glfw-3.3.5 && cmake . && make --silent
ifeq ($(UNAME_S),Darwin)
	gcc $(OBJ) -o $(NAME) -framework Cocoa -framework OpenGL -framework QuartzCore -framework IOKit $(LIBS)
else
	gcc $(OBJ) -o $(NAME) $(LIBS)
endif

clean:
	rm -rf $(OBJDIR) $(DEPDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all

-include $(DEP)
