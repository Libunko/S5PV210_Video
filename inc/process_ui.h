#ifndef _PROCESS_UI_H
#define _PROCESS_UI_H

#include <stdio.h>
#include <string.h>


#include "../inc/player.h"
#include "../inc/retro_snaker.h"

//主页相关
unsigned char ui_status;
#define UI_HOME 0
#define UI_CAM  1
#define UI_PLA  2
#define UI_PIC  3
#define UI_LOCK 4
#define UI_SNAKER 5
#define UI_INF  6

//图片相关
unsigned char pic_status;
unsigned char pic_status_last;
#define PICTURE_NUM 3

//播放相关
unsigned char pla_status;
unsigned char pla_num;
unsigned char pla_status_last;
#define SONG_NUM        5
#define PLA_PLAY        1
#define PLA_PAUSE       2
#define PLA_STOP        3
#define PLA_CONTINUE    4
#define PLA_FIRSTINSET  5

//锁屏相关
unsigned char key_value;
unsigned char key_status;
unsigned char pw_count;
char inputpw[128];

// //贪食蛇游戏相关
// char input;

/******************************
 * 
 *      调用函数区
 * 
 * ****************************/
int pro_ui_home(void);
int pro_ui_cam(void);
int pro_ui_pla(void);
int pro_ui_pic(void);
int pro_ui_lock(void);
int pro_ui_inf(void);


#endif 
