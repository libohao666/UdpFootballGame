/*************************************************************************
	> File Name: server_exit.c
	> Author: lbh
	> Mail: 2052658718@qq.com
	> Created Time: 2020年06月13日 星期六 14时44分37秒
 ************************************************************************/

#include "head.h"
#include <signal.h>
#define MAX 50
extern struct User *rteam, *bteam;

void server_exit(int signum) {
	struct FootBallMsg msg;
	msg.type = FT_FIN;
	for (int i = 0; i < MAX; i++) {
		if (rteam[i].online) {
			send(rteam[i].fd, (void *)&msg, sizeof(msg), 0);
		}
		if (bteam[i].online) {
			send(bteam[i].fd, (void *)&msg, sizeof(msg), 0);
		}
	}
	endwin();
	DBG(RED"Server stopped!\n"NONE);
	exit(0);
}
