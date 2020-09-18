#include "head.h"
#include "datatype.h"
#include "udp_epoll.h"
#include "game.h"
#include "show_data_stream.h"

#define MAX 50
extern struct User *rteam, *bteam;
extern int repollfd, bepollfd;

void heart_beat_team(struct User *team) {
	struct FootBallMsg msg;
	msg.type = FT_TEST;
	for (int i = 0; i < MAX; i++) {
		if (team[i].online) {
			if (!team[i].flag) {
				team[i].online = 0;
				continue;
			}
			send(team[i].fd, (void *)&msg, sizeof(msg), 0);
			team[i].flag--;
			if (team[i].flag <= 0) {
				show_data_stream('e');
				char tmp[512] = {0};
				sprintf("%s is removed from list.", team[i].name);
				Show_Message( , NULL, tmp, 1);
				team[i].online = 0;
				int epollfd_tmp = (team[i].team ? bepollfd : repollfd);
				del_event(epollfd_tmp, team[i].fd);
			}
		}
	}
}

void *heart_beat(void *arg) {
	while (1) {
		sleep(10);
		heart_beat_team(bteam);
		heart_beat_team(rteam);
	}
}
