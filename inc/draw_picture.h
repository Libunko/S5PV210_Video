#ifndef _DRWA_PICTURE_H
#define _DRWA_PICTURE_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdlib.h>

#define WIDTH_PIXEL         800
#define HEIGHT_PIXEL        480
#define SCREEN_SIZE_BYTE    WIDTH_PIXEL*HEIGHT_PIXEL*4
#define BMP_HAND_SIZE       54
#define BMP_BUF_SIZE        WIDTH_PIXEL*HEIGHT_PIXEL*3
#define BMP_PIX_SIZE        3


void draw_picture(char *bmp_path, unsigned int x, unsigned int y);

#endif 
