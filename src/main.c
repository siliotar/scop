#include "scop.h"
#include "objChooser.h"

int main(int argc, char** argv)
{
	char	buff[50];
	t_scop	scop;

	if (argc < 2)
	{
		objChooser(buff);
		initScop(&scop, buff);
	}
	else
		initScop(&scop, argv[1]);

	initMainVertexArray(&scop);
	initInterfaceVertexArray(&scop);

	displayUsage();

	mainLoop(&scop);

	destructScop(&scop);
	return 0;
}
