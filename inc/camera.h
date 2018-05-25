#ifndef _CAMERA_H
#define _CAMERA_H

#include <stdio.h>   	//printf scanf
#include <fcntl.h>		//open write read lseek close  	 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "jpeglib.h"
#include "api_v4l2.h"
#include "process_ui.h"


#define LCD_WIDTH  			800
#define LCD_HEIGHT 			480
#define FB_SIZE				(LCD_WIDTH * LCD_HEIGHT * 4)

int camera(void);

#endif
