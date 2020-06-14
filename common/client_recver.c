/*************************************************************************
	> File Name: client_recver.c
	> Author: lbh
	> Mail: 2052658718@qq.com
	> Created Time: 2020年06月13日 星期六 14时13分03秒
 ************************************************************************/

#include "head.h"
#include "game.h"

extern int sockfd;

void *client_recv(void *arg) {
	while (1) {
		struct FootBallMsg msg;
		struct User user;
		bzero(&msg, sizeof(msg));
		recv(sockfd, (void *)&msg, sizeof(msg), 0);
		strcpy(user.name, msg.name);
		user.team = msg.team;
		if (msg.type & FT_TEST) {
			DBG(RED"HeartBeat from Server 心跳\n"NONE);
			msg.type = FT_ACK;
			send(sockfd, (void *)&msg, sizeof(msg), 0);
		} else if (msg.type & FT_MSG) {
			DBG(GREEN"%s : "NONE"%s\n", user.name, msg.msg);
			Show_Message( , &user, msg.msg, 0);
		} else if (msg.type & FT_WALL) {
			DBG(GREEN"Server Msg: "NONE"%s\n", msg.msg);
			Show_Message( , NULL, msg.msg, 1);
		} else if (msg.type & FT_FIN) {
			DBG(GREEN"Server is going to stop!\n"NONE);
			endwin();	
			exit(0);
		} else {
			DBG(GREEN"Server Msg :"NONE"Unsupport Message Type.\n");
		}
	}
}
