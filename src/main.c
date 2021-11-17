#include "scop.h"

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Usage: ./scop name_of_object_file\n");
		return 0;
	}

	t_scop	scop;
	initScop(&scop, argv[1]);

	initMainVertexArray(&scop);
	initInterfaceVertexArray(&scop);

	displayUsage();

	mainLoop(&scop);

	destructScop(&scop);
	return 0;
}
