#include "../inc/camera.h"


static char g_color_buf[FB_SIZE]={0};

static int  lcd_fd;
static int *mem_p;

unsigned char exit_status = 1;

/****************************************************
 *函数名称:file_size_get
 *输入参数:pfile_path	-文件路径
 *返 回 值:-1		-失败
		   其他值	-文件大小
 *说	明:获取文件大小
 ****************************************************/
unsigned long file_size_get(const char *pfile_path)
{
	unsigned long filesize = -1;	
	struct stat statbuff;
	
	if(stat(pfile_path, &statbuff) < 0)
	{
		return filesize;
	}
	else
	{
		filesize = statbuff.st_size;
	}
	
	return filesize;
}

//初始化LCD
int lcd_open(void)
{
	 lcd_fd = open("/dev/fb0", O_RDWR);
	
	if(lcd_fd<0)
	{
			printf("open lcd error\n");
			return -1;
	}

	mem_p  = (int *)mmap(	NULL, 					//映射区的开始地址，设置为NULL时表示由系统决定映射区的起始地址
									FB_SIZE, 				//映射区的长度
									PROT_READ|PROT_WRITE, 	//内容可以被读取和写入
									MAP_SHARED,				//共享内存
									lcd_fd, 				//有效的文件描述词
									0						//被映射对象内容的起点
								);

	return lcd_fd;

}

// //LCD画点
void lcd_draw_point(unsigned int x,unsigned int y, unsigned int color)
{
	*(mem_p+y*800+x)=color;
}


// /*

// x			:起点
// y			:起点
// pjpg_path	：图片路径
// pjpg_buf    ：摄像头获取的图像数据，如果你没用摄像头一般设为NULL
// jpg_buf_size：摄像头数据的大小,默认设为0
// */
int lcd_draw_jpg(unsigned int x,unsigned int y,const char *pjpg_path,char *pjpg_buf,unsigned int jpg_buf_size)  
{
	/*定义解码对象，错误处理对象*/
	struct 	jpeg_decompress_struct 	cinfo;
	struct 	jpeg_error_mgr 			jerr;	
	
	char 	*pcolor_buf = g_color_buf;
	char 	*pjpg;
	
	unsigned int 	i=0;
	unsigned int	color =0;
	unsigned int	count =0;
	
	unsigned int 	x_s = x;
	unsigned int 	x_e ;	
	unsigned int 	y_e ;
	
			 int	jpg_fd;
	unsigned int 	jpg_size;
	
	unsigned int 	jpg_width;
	unsigned int 	jpg_height;
	

	if(pjpg_path!=NULL)
	{
		/* 申请jpg资源，权限可读可写 */	
		jpg_fd=open(pjpg_path,O_RDWR);
		
		if(jpg_fd == -1)
		{
		   printf("open %s error\n",pjpg_path);
		   
		   return -1;	
		}	
		
		/* 获取jpg文件的大小 */
		jpg_size=file_size_get(pjpg_path);	

		/* 为jpg文件申请内存空间 */	
		pjpg = malloc(jpg_size);

		/* 读取jpg文件所有内容到内存 */		
		read(jpg_fd,pjpg,jpg_size);
	}
	else
	{
		jpg_size = jpg_buf_size;
		
		pjpg = pjpg_buf;
	}

	/*注册出错处理*/
	cinfo.err = jpeg_std_error(&jerr);

	/*创建解码*/
	jpeg_create_decompress(&cinfo);

	/*直接解码内存数据*/		
	jpeg_mem_src(&cinfo,pjpg,jpg_size);
	
	/*读文件头*/
	jpeg_read_header(&cinfo, TRUE);

	/*开始解码*/
	jpeg_start_decompress(&cinfo);	
	//打印获取图片的实际高度和宽度
	//printf("cinfo.output_height = %d\n,cinfo.output_width = %d\n",cinfo.output_height,cinfo.output_width);
	
	
	x_e	= x_s+cinfo.output_width;
	y_e	= y  +cinfo.output_height;	
	//printf("x_e = %d , y_e = %d\n",x_e,y_e);
	/*读解码数据*/
	while(cinfo.output_scanline < cinfo.output_height )
	{		
		pcolor_buf = g_color_buf;
		
		/* 读取jpg一行的rgb值 */
		jpeg_read_scanlines(&cinfo,(JSAMPARRAY)&pcolor_buf,1);
		
		for(i=0; i<cinfo.output_width; i++)
		{
			/* 获取rgb值 */
			color = 		*(pcolor_buf+2);
			color = color | *(pcolor_buf+1)<<8;
			color = color | *(pcolor_buf)<<16;
			
			/* 显示像素点 */
			lcd_draw_point(x,y,color);
			
			pcolor_buf +=3;
			
			x++;
		}
		
		/* 换行 */
		y++;			
		
		x = x_s;
		//printf("x=%d  y=%d\n",x,y);
		
	}		
	
	
				
	/*解码完成*/
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	if(pjpg_path!=NULL)
	{
		/* 关闭jpg文件 */
		close(jpg_fd);	
		
		/* 释放jpg文件内存空间 */
		free(pjpg);		
	}	
	return 0;
}



// //LCD关闭
void lcd_close(void)
{
	
	/* 取消内存映射 */
	munmap(mem_p, FB_SIZE);
	
	/* 关闭LCD设备 */
	close(lcd_fd);
}

int camera(void)
{
    lcd_open();
    //摄像头初始化
    /* 打开摄像头设备 */
    linux_v4l2_device_init("/dev/video3");
    usleep(10000);
    /* 启动摄像头捕捉 */
    linux_v4l2_start_capturing();
    //获取摄像头数据
	
    FrameBuffer camera_buf;

    while(ui_status == UI_CAM)
    {
        linux_v4l2_get_fream(&camera_buf);
        /* 在x=80,y=0,以正常大小显示摄像头的数据 */
        lcd_draw_jpg(80,0,NULL,camera_buf.buf,camera_buf.length);
    }
    linux_v4l2_stop_capturing();
    linux_v4l2_device_uinit();
    lcd_close();
    return 0;
}

