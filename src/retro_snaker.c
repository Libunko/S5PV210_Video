#include "../inc/retro_snaker.h"


#define PIXSIZE			10
#define SCREENWIDTH		800/PIXSIZE
#define SCREENHEIGHT	480/PIXSIZE


unsigned char snaker_length;		//贪吃蛇长度
unsigned int num_srand;		//随机数种子
unsigned char i;
unsigned char snaker_buff[128] = {0};


void make_rand(void);
void game_over(void);
int show_snaker(unsigned char x, unsigned char y, unsigned char color);


struct
{
	unsigned char x;
	unsigned char y;
	unsigned char color;
}snaker[SCREENWIDTH*SCREENHEIGHT], food;

void snaker_init(void)
{
	direction = DOWN;//第一次进去游戏时防止出错
	draw_picture("./pic/ico/black.bmp", 0, 0);
	snaker[0].x = 10;
	snaker[0].y = 32;
	snaker[1].x = 9;
	snaker[1].y = 32;
	snaker[2].x = 8;
	snaker[2].y = 32;
	snaker[3].x = 7;
	snaker[3].y = 32;
	snaker[4].x = 6;
	snaker[4].y = 32;
	snaker[5].x = 5;
	snaker[5].y = 32;
	snaker[6].x = 4;
	snaker[6].y = 32;
	snaker_length = 7;
	for(i=0;i<snaker_length;i++)
		snaker[i].color = 1;
	make_rand();		//产生food随机数
}
unsigned char move(void)
{
	if(num_srand++ == 10000)
		num_srand = 0;	 //随机数种子
	printf("snaker[0].x = %d snaker[0].y = %d snaker_length = %d direction = %d\r\n",snaker[0].x,snaker[0].y,snaker_length,direction);
	//判断有没有碰壁
	if((snaker[0].x<=0)||(snaker[0].x>=(SCREENWIDTH-1))||(snaker[0].y<=0)||(snaker[0].y>=(SCREENHEIGHT-1)))
	{
		printf("------------touch wall----------\n");
		game_over();		//游戏结束
			
		return 1;
	}
	for(i=1;i<snaker_length;i++)	//判断有没有碰蛇身
	{
		if(snaker[0].x == snaker[i].x)
		{
			if(snaker[0].y == snaker[i].y)
			{
				printf("------------touch body----------\n");
				game_over();		//游戏结束
				return 1;
			}
		}
	}
	if(snaker[0].x==food.x&&snaker[0].y==food.y)
		make_rand();				//产生随机数
		
	for(i=snaker_length;i>0;i--)	//蛇头数据向蛇尾移动
	{
		snaker[i].x = snaker[i-1].x;
		snaker[i].y = snaker[i-1].y;
		snaker[i].color = snaker[i-1].color;
	}
	switch(direction)				//判断按键方向
	{
		case UP:	snaker[0].y--;break;
		case DOWN:	snaker[0].y++;break;
		case LEFT:	snaker[0].x--;break;
		case RIGHT:	snaker[0].x++;break;
		default :direction = DOWN;break;//第一次进去游戏时防止出错
	}
	snaker[snaker_length].color = 0;	//反白蛇尾


	draw_picture("./pic/ico/snaker_hand.bmp", PIXSIZE*snaker[0].x, PIXSIZE*snaker[0].y);		//更新蛇位置  蛇头颜色不同
	draw_picture("./pic/ico/snaker_hand.bmp", PIXSIZE*snaker[1].x, PIXSIZE*snaker[1].y);		//更新蛇位置  蛇头颜色不同
	for(i=2;i<=snaker_length;i++)		//更新蛇位置
	{
		show_snaker(snaker[i].x, snaker[i].y, snaker[i].color);
	}
	return 1;
}
void make_rand(void)
{
	srand(num_srand);
	snaker_length++;
	food.x = rand()%(SCREENWIDTH-3)+2;//产生随机数 2 ~  SCREENWIDTH-3
	food.y = rand()%(SCREENHEIGHT-3)+2;//产生随机数 2 ~ SCREENHEIGHT-3
	food.color = 1;
	printf("food.x = %d, food.y = %d\n", food.x, food.y);
	draw_picture("./pic/ico/snaker_food.bmp", PIXSIZE*food.x, PIXSIZE*food.y);
}
void game_over(void)
{
	printf("GAME OVER !\r\n");
	direction = GAME_END;
	usleep(1000*100);
	draw_picture("./pic/ico/game_over.bmp", 199, 89);
	while(1)
	{
		if(GAME_START == direction)
		{
			snaker_init();
			// ui_status = UI_SNAKER;
			break;
		}
		else if(GAME_OVER == direction)
		{
			ui_status = UI_HOME;
			break;
		}
	}
}
int show_snaker(unsigned char x, unsigned char y, unsigned char color)
{
	sprintf(snaker_buff, "./pic/ico/snaker_%d.bmp", color);
	draw_picture(snaker_buff, PIXSIZE*x, PIXSIZE*y);
	return 0;
}