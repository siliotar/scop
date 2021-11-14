#ifndef BMP_READER_H
# define BMP_READER_H

# include <stdio.h>
# include <stdlib.h>

unsigned char* read_bmp(const char* fname, int* _w, int* _h, int* _bpp);

#endif
