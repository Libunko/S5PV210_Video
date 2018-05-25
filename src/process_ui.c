#include "../inc/process_ui.h"

#include <termios.h>

char pic_buff[128] = {0};
char pla_buff[128] = {0};
char point_buff[128] = {0};

/*********************************
 * 
 * 密码区
 * 
 * *******************************/
char password[128] = "147258";
#define PWLENGTH 6	//密码位数

int pro_ui_home(void)
{
	draw_picture("./pic/ico/home_background.bmp", 0, 0);
	
	draw_picture("./pic/ico/cam.bmp", 108, 90);
	draw_picture("./pic/ico/game.bmp", 108, 280);
	draw_picture("./pic/ico/pla.bmp", 334, 90);
	draw_picture("./pic/ico/inf.bmp", 339, 280);
	draw_picture("./pic/ico/pic.bmp", 570, 90);
	draw_picture("./pic/ico/lock.bmp", 570, 280);
	while(ui_status == UI_HOME);
	return 0;
}

int pro_ui_cam(void)
{
	draw_picture("./pic/ico/black.bmp", 0, 0);
	draw_picture("./pic/ico/cam_exit.bmp", 20, 20);
	camera();
	while(ui_status == UI_CAM);
	return 0;
}

int pro_ui_pla(void)
{
	draw_picture("./pic/ico/home_background.bmp", 0, 0);
    draw_picture("./pic/ico/exit.bmp", 20, 20);
	draw_picture("./pic/ico/home_ico.bmp", 90, 20);
	draw_picture("./pic/ico/next_song.bmp", 549, 239);
	draw_picture("./pic/ico/last_song.bmp", 149, 239);

	if(PLA_PLAY == pla_status || PLA_CONTINUE == pla_status)
		draw_picture("./pic/ico/pause_button.bmp", 299, 139);
	else
		draw_picture("./pic/ico/play_button.bmp", 299, 139);

	sprintf(pla_buff, "./music/%02d.mp3", pla_num);

	switch(pla_status)
	{
		case PLA_CONTINUE:	continue_music();break;
		case PLA_PLAY:		play_music(pla_buff);break;
		case PLA_PAUSE:		pause_music();break;
		case PLA_STOP:		stop_music();break;
		default:break;
	}

	pla_status_last = pla_status;
	while(pla_status == pla_status_last && ui_status == UI_PLA);
	return 0;
}

int pro_ui_pic(void)
{
	sprintf(pic_buff, "./pic/photo/%02d.bmp", pic_status);
	draw_picture(pic_buff, 0, 0);

	pic_status_last = pic_status;
	while(pic_status == pic_status_last && ui_status == UI_PIC);
	return 0;
}

int pro_ui_lock(void)
{
	draw_picture("./pic/ico/key.bmp", 0, 0);
	while(1)
	{
		if(key_status)
		{
			if(++pw_count >= PWLENGTH)
			{
				sprintf(inputpw, "%s%d", inputpw, key_value);

				if(strcmp(password, inputpw) == 0)	//密码正确跳出while
				{
					pw_count = 0;
					key_status = 0;
					memset(inputpw, 0, sizeof(inputpw));
					break;
				}

				pw_count = 0;
				memset(inputpw, 0, sizeof(inputpw));
				draw_picture("./pic/ico/pw_error.bmp", 0, 0);
				usleep(500*1000);
				// draw_picture("./pic/ico/key.bmp", 0, 0);
				key_status = 0;
				return;
			}
			printf("pw_count: %d\n", pw_count);
			sprintf(point_buff, "./pic/ico/point_%02d.bmp", pw_count);
			draw_picture(point_buff, 0, 119);
			sprintf(inputpw, "%s%d", inputpw, key_value);
			printf("inputpw: %s\n", inputpw);
			key_status = 0;
		}
	}
	show_gif(2);
	ui_status = UI_HOME;
	return 0;
}
int pro_ui_snaker(void)
{
	snaker_init();
	while(ui_status == UI_SNAKER)
	{
		move();
		usleep(1000*100);
	}
	
	return 0;
}

int pro_ui_inf(void)
{
	draw_picture("./pic/ico/black.bmp", 0, 0);
	draw_picture("./pic/ico/cam_exit.bmp", 20, 20);
	draw_picture("./pic/ico/information.bmp", 199, 89);
	draw_picture("./pic/ico/panda.bmp", 671, 352);
	while(ui_status == UI_INF);
	return 0;
}
