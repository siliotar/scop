#include "bmp_reader.h"

unsigned char* read_bmp(const char* fname, int* _w, int* _h, int* _bpp)
{
	unsigned char head[54];
	FILE* f = fopen(fname, "rb");
	if (!f)
		return 0;

	fread(head, 1, 54, f);

	int dataPos = head[10] + (((int)head[11]) << 8) + (((int)head[12]) << 16) + (((int)head[13]) << 24);
	int w = head[18] + (((int)head[19]) << 8) + (((int)head[20]) << 16) + (((int)head[21]) << 24);
	int h = head[22] + (((int)head[23]) << 8) + (((int)head[24]) << 16) + (((int)head[25]) << 24);
	int bpp = (short)head[28] / 8;

	unsigned char* data = malloc(w * h * bpp);
	if (!data)
		exit(EXIT_FAILURE);

	fseek(f, dataPos, SEEK_SET);

	fread(data, 1, w * h * bpp, f);

	int tmp, pos;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			pos = i * w * bpp + j * bpp;
			tmp = data[pos];
			data[pos] = data[pos + 2];
			data[pos + 2] = tmp;
		}
	}

	*_w = w;
	*_h = h;
	*_bpp = bpp;
	return data;
}
