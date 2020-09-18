#include "head.h"

extern WINDOW *Write;
extern int sockfd;
int strength = 1;

void kick_ball() {
	struct FootBallMsg msg;
	msg.type = FT_CTL;
	msg.ctl.action = ACTION_KICK;
	msg.ctl.strength = strength;
	send(sockfd, (void *)&msg, sizeof(msg), 0);
}

void carry_ball() {
	struct FootBallMsg msg;
	msg.type = FT_CTL;
	msg.ctl.action = ACTION_CARRY;
	send(sockfd, (void *)&msg, sizeof(msg), 0);
}

void stop_ball() {
	struct FootBallMsg msg;
	msg.type = FT_CTL;
	msg.ctl.action = ACTION_STOP;
	send(sockfd, (void *)&msg, sizeof(msg), 0);
}

void show_strength() {
	int maxx, maxy, loc = 2, dir = 1;
	int arr[5] = {1, 2, 3, 2, 1};
	getmaxyx(Write, maxy, maxx);
	for (int i = 2; i < maxx - 2; i++) {
		if (i < maxx / 5 || i > 4 * maxx / 5) {
			wattron(Write, COLOR_PAIR(7));
		} else if (i < 2 * (maxx / 5) || i > 3 * (maxx / 5)) {
			wattron(Write, COLOR_PAIR(9));
		} else {
			wattron(Write, COLOR_PAIR(8));
		}
		mvwprintw(Write, 2, i, " ");
	}
	wrefresh(Write);
	make_nonblock(0);
	while (1) {
		char c = getchar();
		if ( c != -1) {
			if (c == ' ' || c == 'k') {
				//arr[loc / 5]
				struct FootBallMsg msg;
				msg.type = FT_CTL;
				msg.ctl.action = ACTION_KICK;
				strength = msg.ctl.strength = arr[loc / (maxx / 5)];
				send(sockfd, (void *)&msg, sizeof(msg), 0);
				for (int i = 0; i <= court.width; i++) {
					mvwprintw(Write, 1, i, " ");
					mvwprintw(Write, 3, i, " ");
				}
				make_block(0);
				break;
			}
		} 
		usleep(30000);
		if (loc >= maxx - 2) dir = -1;
		if (loc <= 2) dir = 1;
		wattron(Write, COLOR_PAIR(3));
		mvwprintw(Write, 1, loc, " ");
		mvwprintw(Write, 3, loc, " ");
		loc += dir;
		mvwprintw(Write, 1, loc, "|");
		mvwprintw(Write, 3, loc, "|");
		mvwprintw(Write, 4, maxx, " ");
		wrefresh(Write);
	}
}

