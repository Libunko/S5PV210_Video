#include "../inc/draw_picture.h"

int get_bmp_w_h(char * bmp_info_buff,unsigned int *w,unsigned int *h)
{
	//1. 检查传入的指针是否为空
	if(bmp_info_buff == NULL || w == NULL || h == NULL)
    {
		printf("pos NULL\n");
		return -1;
	}
	//2. 偏移到图片的宽度信息(14+4 字节)
	//3. 偏移到图片的高度信息
	//获取宽度信息,转换大小端（bmp格式是大端，linux是小端）
	unsigned int _bmp_w = *(unsigned int *)(bmp_info_buff + 18);   //18 偏移到图片的宽度信息的偏移量
	unsigned int _bmp_h = *(unsigned int *)(bmp_info_buff + 18 + 4); //4 偏移过bmp的宽度信息
	
	// printf("get_bmp_w_h: w = %d, h = %d\n",_bmp_w, _bmp_h);
	
	//4. 赋值给*w，以及*h
	*w = _bmp_w;
	*h = _bmp_h;

	return 0;
}

void write_bmp(char *bmp_path, unsigned int *fb0_buf, unsigned int x, unsigned int y)
{
    int bmp_fd = 0;
    char bmp_buf[BMP_BUF_SIZE] = {0};
    char bmp_info_buff[BMP_HAND_SIZE] = {0};
    int i = 0;
    int j = 0;

	unsigned int bmp_w = 0; 
	unsigned int bmp_h = 0;

    unsigned int _bmp_w = 0;
	unsigned int _bmp_h = 0;

    //打开BMP图片
    bmp_fd = open(bmp_path, O_RDWR);
    if(-1 == bmp_fd)
    {
        printf("error: bmp open failed\n");
        exit(1);
    }

	//获取图片的长宽
    
    read(bmp_fd, bmp_info_buff, BMP_HAND_SIZE);
	get_bmp_w_h(bmp_info_buff, &bmp_w, &bmp_h);

    //偏移54字节
    if(-1 == lseek(bmp_fd, BMP_HAND_SIZE, SEEK_SET))
    {
        printf("error: lseek failed\n");
        exit(1);
    }

    //读取图片信息
    int fill_zero_per_line = 4 - ((bmp_w*BMP_PIX_SIZE)%4);//计算bmp每行进行4字节对其后 对每行信息进行填充的0的字节数。
	if(fill_zero_per_line == 4)
	{
		fill_zero_per_line =0;
	}

	// printf("fill_zero_per_line = %d\n", fill_zero_per_line);

    if(-1 ==read(bmp_fd, bmp_buf, bmp_w * bmp_h * BMP_PIX_SIZE +  bmp_h * fill_zero_per_line))
    {
        printf("error: read bmp failed\n");
        exit(1);
    }

	if(bmp_h + y > HEIGHT_PIXEL)
	{
		_bmp_h = HEIGHT_PIXEL - y;
	}
	else
	{
		_bmp_h = bmp_h;
	}

	if(bmp_w + x > WIDTH_PIXEL)
	{
		_bmp_w = WIDTH_PIXEL - x;
	}
	else
	{
		_bmp_w = bmp_w;
	}
    
    //BMP信息写入fb0_buf
	for(i = 0 ;i < _bmp_h; i++) 
	{
		for(j = 0 ;j < _bmp_w; j++)
		{
			fb0_buf[(_bmp_h - 1 - i+y)*WIDTH_PIXEL + (j+x)] = 0x0 <<24 |bmp_buf[(i*bmp_w+j)*BMP_PIX_SIZE + i*fill_zero_per_line  ] << 0 | \
										bmp_buf[(i*bmp_w+j)*BMP_PIX_SIZE + i*fill_zero_per_line + 1] <<8 | \
										bmp_buf[(i*bmp_w+j)*BMP_PIX_SIZE + i*fill_zero_per_line + 2] << 16;
			
		}
	}

    //关闭图片文件
    if(-1 == close(bmp_fd))
    {
        printf("error: bmp close failed/n");
        exit(1);
    }
}

void draw_picture(char *bmp_path, unsigned int x, unsigned int y)
{
    int fb0_fd = 0;

    //打开fb0文件
    fb0_fd = open("/dev/fb0", O_RDWR);
    if(-1 == fb0_fd)
    {
        printf("error: fb0 open failed\n");
        exit(1);
    }

    //内存映射
    unsigned int *fb0_buf = (unsigned int *)mmap(NULL,SCREEN_SIZE_BYTE, PROT_READ|PROT_WRITE, MAP_SHARED, fb0_fd, 0);

    //写入图片信息
    write_bmp(bmp_path, fb0_buf, x, y);

    //解除映射
    if(-1 == munmap(fb0_buf, SCREEN_SIZE_BYTE))
    {
        printf("error: munmap failed\n");
        exit(1);
    }

    //关闭fb0文件
    if(-1 == close(fb0_fd))
    {
        printf("error: fb0 close failed/n");
        exit(1);
    }
}

