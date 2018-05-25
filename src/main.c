#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

//显示图片相关
#include "../inc/tslib.h"
#include "../inc/draw_picture.h"
#include "../inc/process_ui.h"

//摄像头相关
#include "../inc/camera.h"

//音乐相关
#include "../inc/player.h"

void *touch (void *arg);
int show_gif(unsigned char gif);

//锁屏相关
unsigned char key_button[4][3] = {1,2,3,4,5,6,7,8,9,0,0,0};


int main(int argc, char const *argv[])
{
	draw_picture("./pic/ico/black.bmp", 0, 0);
	usleep(500*1000);	//等待500ms

	//初始化状态值
	{
		ui_status = UI_LOCK;
		// ui_status = UI_HOME;
		pic_status = 1;
		pla_num = 1;
		pla_status = PLA_FIRSTINSET;
		pw_count = 0;
		memset(inputpw, 0, sizeof(inputpw));//清空inputpw数组
	}

	pthread_t thread_id;

	show_gif(1);

	if(0 != pthread_create(&thread_id, NULL, touch, NULL))
	{
		printf("pthread_create error\n");
		return -1;
	}

	while(1)
	{
		switch(ui_status)
		{
			case UI_HOME:	pro_ui_home();break;
			case UI_CAM:	pro_ui_cam(); break;
			case UI_PLA:	pro_ui_pla(); break;
			case UI_PIC:	pro_ui_pic(); break;
			case UI_LOCK:	pro_ui_lock(); break;
			case UI_SNAKER: pro_ui_snaker(); break;
			case UI_INF:	pro_ui_inf(); break;
			default:break;
		}
	}
	return 0;
}


void *touch (void *arg)
{
	struct tsdev *ts_dev = ts_open("/dev/event0", 0);
	ts_config(ts_dev);
	struct ts_sample ts_pos;

	while(1)
	{
		ts_read(ts_dev, &ts_pos, 1);
		printf("x = %d,y = %d ,pressure =  %d\n", ts_pos.x, ts_pos.y, ts_pos.pressure );
		if(ts_pos.pressure > 0)
		{
			switch(ui_status)
			{
				case UI_HOME:
				{
					//坐标处理
					if(ts_pos.x > 532 && ts_pos.x < 800 && ts_pos.y > 240 && ts_pos.y < 480)
					{
						ui_status = UI_LOCK;
					}
					if(ts_pos.x > 50 && ts_pos.x < 266 && ts_pos.y > 50 && ts_pos.y < 240)	//(50,50)防误触
					{
						ui_status = UI_CAM;
					}
					if(ts_pos.x > 266 && ts_pos.x < 532 && ts_pos.y > 0 && ts_pos.y < 240)
					{
						ui_status = UI_PLA;
					}
					if(ts_pos.x > 532 && ts_pos.x < 800 && ts_pos.y > 0 && ts_pos.y < 240)
					{
						ui_status = UI_PIC;
					}
					if(ts_pos.x > 0 && ts_pos.x < 266 && ts_pos.y > 240 && ts_pos.y < 480)
					{
						ui_status = UI_SNAKER;
						sleep(1);	//等待初始化好
					}
					if(ts_pos.x > 266 && ts_pos.x < 532 && ts_pos.y > 240 && ts_pos.y < 480)
					{
						ui_status = UI_INF;
					}
					break;
				}
				case UI_CAM:
				{
					if(ts_pos.x > 0 && ts_pos.x < 80 && ts_pos.y > 0 && ts_pos.y < 80)
					{
						ui_status = UI_HOME;
					}
					//视频监控界面的 相关处理
					break;
				}
				case UI_PLA:
				{
					//退出
					if(ts_pos.x > 0 && ts_pos.x < 80 && ts_pos.y > 0 && ts_pos.y < 80)
					{
						ui_status = UI_HOME;
						if(PLA_STOP != pla_status)
							stop_music();
						pla_status = PLA_FIRSTINSET;
					}
					//返回home
					if(ts_pos.x > 80 && ts_pos.x < 160 && ts_pos.y > 0 && ts_pos.y < 80)
					{
						ui_status = UI_HOME;
						pla_status = PLA_CONTINUE;
						// pla_status =PLA_FIRSTINSET;
					}

					//上一首歌曲
					if(ts_pos.x > 150 && ts_pos.x < 250 && ts_pos.y > 240 && ts_pos.y < 340)
					{
						if(pla_num++ == SONG_NUM)
							pla_num = 1;
						pla_status = PLA_STOP;
						usleep(500);	//延时使状态标志位相等
						pla_status = PLA_PLAY;
					}

					//下一首歌曲
					if(ts_pos.x > 550 && ts_pos.x < 650 && ts_pos.y > 240 && ts_pos.y < 340)
					{
						if(--pla_num == 0)
							pla_num = SONG_NUM;
						pla_status = PLA_STOP;
						usleep(500);	//延时使状态标志位相等
						pla_status = PLA_PLAY;
					}
					//音乐播放界面的 相关处理

					//暂停按钮
					if(ts_pos.x > 299 && ts_pos.x < 499 && ts_pos.y > 139 && ts_pos.y < 439)
					{
						switch(pla_status)
						{
							case PLA_CONTINUE:	pla_status = PLA_PAUSE;	break;
							case PLA_PLAY:		pla_status = PLA_PAUSE;	break;
							case PLA_PAUSE:		pla_status = PLA_CONTINUE;	break;
							case PLA_STOP:		pla_status = PLA_PLAY;	break;
							default:			pla_status = PLA_PLAY;	break;//第一次进入播放器界面 播放
						}
						
					}
					
					break;
				}
				case UI_PIC:
				{
					if(ts_pos.x > 0 && ts_pos.x < 90 && ts_pos.y > 0 && ts_pos.y < 90)
					{
						ui_status = UI_HOME;
					}
					//相册界面的 相关处理

					//上一张图片
					if(ts_pos.x > 0 && ts_pos.x < 300 && ts_pos.y > 99 && ts_pos.y < 379)
					{
						if(pic_status++ == PICTURE_NUM)
							pic_status = 1;
						usleep(5000);
					}

					//下一张图片
					if(ts_pos.x > 499 && ts_pos.x < 799 && ts_pos.y > 99 && ts_pos.y < 379)
					{
						if(--pic_status == 0)
							pic_status = PICTURE_NUM;
						usleep(5000);
					}

					break;
				}
				case UI_LOCK:
				{
					if(ts_pos.x > 399)
					{
						key_value = key_button[ts_pos.y/120][(ts_pos.x-400)/133];
						// usleep(100*1000);
						key_status = 1;
						// usleep(10);
						printf("key_value: %d\n", key_value);
					}
					//清空键盘
					if(ts_pos.x > 0 && ts_pos.x < 400 && ts_pos.y > 360 && ts_pos.y < 480)
					{
						pw_count = 0;
						key_status = 0;
						memset(inputpw, 0, sizeof(inputpw));
						draw_picture("./pic/ico/point_00.bmp", 0, 119);
					}
					break;
				}
				case UI_SNAKER:
				{
					if(direction != GAME_END)
					{
						//退出
						if(ts_pos.x > 0 && ts_pos.x < 50 && ts_pos.y > 0 && ts_pos.y < 50)
						{
							direction = GAME_OVER;
							ui_status = UI_HOME;
						}

						if(ts_pos.x > 560 && ts_pos.x < 800 && ts_pos.y > 0 && ts_pos.y < 480)
						{
							if(direction != LEFT)
								direction = RIGHT;
						}
						if(ts_pos.x > 0 && ts_pos.x < 240 && ts_pos.y >0  && ts_pos.y < 480)
						{
							if(direction != RIGHT)
								direction = LEFT;
						}
						if(ts_pos.x > 280 && ts_pos.x < 520 && ts_pos.y > 240 && ts_pos.y < 480)
						{
							if(direction != UP)
								direction = DOWN;
						}
						if(ts_pos.x > 280 && ts_pos.x < 520 && ts_pos.y > 0 && ts_pos.y < 240)
						{
							if(direction != DOWN)
								direction = UP;
						}
						usleep(1000*100);	//延时防止方向变了蛇身未变而引起的错误（理论应该与蛇身延时相等即1000*100）    该值 < 1000*1000 时 可能会贪吃蛇会莫名死亡
						//
					}
					else
					{
						if(ts_pos.x > 200 && ts_pos.x < 400 && ts_pos.y > 240 && ts_pos.y < 390)
						{
							direction = GAME_START;
						}
						if(ts_pos.x > 400 && ts_pos.x < 600 && ts_pos.y > 240 && ts_pos.y < 390)
						{
							direction = GAME_OVER;
							ui_status = UI_HOME;
						}
					}
					break;
				}
				case UI_INF:
				{
					if(ts_pos.x > 0 && ts_pos.x < 80 && ts_pos.y > 0 && ts_pos.y < 80)
					{
						ui_status = UI_HOME;
					}
					break;
				}
				default:
				{
					break;
				}
			}
		}
	}
	return NULL;
}

int show_gif(unsigned char gif)
{
	unsigned char i = 0;
	unsigned char gif_buff[128] = {0};

	switch(gif)
	{
		case 1:
		{
			draw_picture("./pic/ico/home_background.bmp", 0, 0);
			for(i=0;i<39;i++)
			{
				sprintf(gif_buff, "./pic/gif/IMG000%02d.bmp", i);
				draw_picture(gif_buff, 279, 119);
				usleep(50*1000);
			}
			for(i=38;i>0;i--)
			{
				sprintf(gif_buff, "./pic/gif/IMG000%02d.bmp", i);
				draw_picture(gif_buff, 279, 119);
				usleep(50*1000);
			}
			break;
		}
		case 2:
		{
			draw_picture("./pic/ico/home_background.bmp", 0, 0);
			for(i=0;i<15;i++)
			{
				sprintf(gif_buff, "./pic/gif2/IMG000%02d.bmp", i);
				draw_picture(gif_buff, 39, 0);
				usleep(50*1000);
			}
			break;
		}
		default: break;
	}
	return 0;
}