#include "../inc/player.h"

int play_music(char *music_name)
{
	stop_music();
	char cmd_buff[128] = {0};
	sprintf(cmd_buff,"madplay %s -r &", music_name);
	system(cmd_buff);
	return 0;
}


int stop_music()
{

	system("killall -KILL madplay");
	return 0;
}


int pause_music()
{

	system("killall -STOP madplay");
	return 0;
}

int continue_music()
{

	system("killall -CONT madplay");
	return 0;
}