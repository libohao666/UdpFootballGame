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
extern struct Bpoint ball;
extern int bscore, rscore;

cJSON *game_msg() {
	cJSON *root = cJSON_CreateObject();
	cJSON *c_ball = cJSON_CreateObject();
	cJSON *users = cJSON_CreateArray();
	cJSON *c_score = cJSON_CreateObject();
	
	cJSON_AddItemToObject(root, "name", cJSON_CreateString("test"));
	cJSON_AddItemToObject(root, "ball", c_ball);
	cJSON_AddItemToObject(root, "score", c_score);
	cJSON_AddItemToObject(root, "users", users);
	
	cJSON_AddNumberToObject(c_ball, "x", ball.x);
	cJSON_AddNumberToObject(c_ball, "y", ball.y);

	cJSON_AddNumberToObject(c_score, "bscore", bscore);
	cJSON_AddNumberToObject(c_score, "rscore", rscore);

	for (int i = 0; i < MAX; i++) {
		if (rteam[i].online) {
			cJSON *user = cJSON_CreateObject();
			cJSON_AddNumberToObject(user, "team", rteam[i].team);
			cJSON_AddStringToObject(user, "name", rteam[i].name);
			cJSON_AddNumberToObject(user, "locx", rteam[i].loc.x);
			cJSON_AddNumberToObject(user, "locy", rteam[i].loc.y);
			cJSON_AddItemToArray(users, user);
		}
		if (bteam[i].online) {
			cJSON *user = cJSON_CreateObject();
			cJSON_AddNumberToObject(user, "team", bteam[i].team);
			cJSON_AddStringToObject(user, "name", bteam[i].name);
			cJSON_AddNumberToObject(user, "locx", bteam[i].loc.x);
			cJSON_AddNumberToObject(user, "locy", bteam[i].loc.y);
			cJSON_AddItemToArray(users, user);
		}
	}
	return root;
}

cJSON *score_msg(char *name) {
	cJSON *root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "name", cJSON_CreateString(name));
	return root;
}

void score_send(char *name) {
	struct FootBallMsg msg;
	msg.type = FT_SCORE;
	for (int i = 0; i < MAX; i++) {
		if (rteam[i].online) {
			strcpy(msg.msg, cJSON_Print(score_msg(name)));
			send(rteam[i].fd, (void *)&msg, sizeof(msg), 0);
		}
		if (bteam[i].online) {
			strcpy(msg.msg, cJSON_Print(score_msg(name)));
			send(bteam[i].fd, (void *)&msg, sizeof(msg), 0);
		}
	}
}

void server_send() {
	struct FootBallMsg msg;
	msg.type = FT_GAME;
	for (int i = 0; i < MAX; i++) {
		if (rteam[i].online) {
			strcpy(msg.msg, cJSON_Print(game_msg()));
			send(rteam[i].fd, (void *)&msg, sizeof(msg), 0);
		}
		if (bteam[i].online) {
			strcpy(msg.msg, cJSON_Print(game_msg()));
			send(bteam[i].fd, (void *)&msg, sizeof(msg), 0);
		}
	}
}

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
