#include "scop.h"

void	initScop(t_scop *scop)
{
	scop->anglex = 0.0f;
	scop->angley = 0.0f;
	scop->anglez = 0.0f;
	scop->texTrans = 0.0f;
	scop->texSign = -1;
	scop->tempTex = 0;
	scop->defaultTexture.data = 0;
	scop->customTexture.data = 0;
}