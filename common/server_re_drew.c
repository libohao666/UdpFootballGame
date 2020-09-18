/*************************************************************************
	> File Name: server_re_drew.c
	> Author: lbh
	> Mail: 2052658718@qq.com
	> Created Time: 2020年06月14日 星期日 20时29分32秒
 ************************************************************************/

#include "head.h"
#include "game.h"
#include "server_exit.h"
#define MAX 50

extern struct User *rteam,*bteam;
extern WINDOW *Football, *Football_t, *Score;
extern struct Bpoint ball;
extern struct BallStatus ball_status;
extern struct Map court;
extern int rscore, bscore;

void re_drew_player(int team, char *name, struct Point *loc) {
	char p = 'K';
	char blank[20] = {0};
	wattron(Football, COLOR_PAIR((team ? 6 : 2)));
	w_gotoxy_putc(Football, loc->x, loc->y, p);
	w_gotoxy_puts(Football, loc->x + 1, loc->y - 1, name);
	wattroff(Football, COLOR_PAIR((team ? 6 : 2)));
}

void re_drew_team(struct User *team) {
	for (int i = 0; i < MAX ; i++) {
		if (!team[i].online) continue;
		re_drew_player(team[i].team, team[i].name, &team[i].loc);
	}
}

void re_drew_ball() {
	if (ball_status.carry) {
		ball.x = ball_status.user->loc.x;
		ball.y = ball_status.user->loc.y;
		if (ball_status.user->team) {
			ball.x -= 1;
		} else ball.x += 1;
	}
	if (ball_status.v.x != 0 || ball_status.v.y != 0) {
		ball.x += ball_status.v.x * 0.1 + ball_status.a.x * 0.5 * 0.01;
		ball.y += ball_status.v.y * 0.1 + ball_status.a.y * 0.5 * 0.01;
		ball_status.v.x += ball_status.a.x * 0.1;
		ball_status.v.y += ball_status.a.y * 0.1;
	}
	if (abs(ball_status.v.x) < 0.5) {
		ball_status.v.x = ball_status.a.x = 0;
	}
	if (abs(ball_status.v.y) < 0.5) {
		ball_status.v.y = ball_status.a.y = 0;
	}
	if (ball_status.v.x || ball_status.v.y) {
		char tmp[512] = {0};
		sprintf(tmp, "a(%lf, %lf) v(%lf, %lf)", ball_status.a.x, ball_status.a.y, ball_status.v.x, ball_status.v.y);
		//Show_Message( , NULL, tmp, 1);
	}
	if (ball.x < 0) {
		if (ball.y >= (2 * court.height / 5) && ball.y <= (3 * court.height / 5)) {
			ball.x = court.width / 2;
			ball.y = court.height / 2;
			bscore += 1;
			char tmp[512] = {0};
			sprintf(tmp, "%s get 1 score", ball_status.name);
			score_send(ball_status.name);
			Show_Message( , NULL, tmp, 1);

		} else {
			ball.x = 1;
		}
		ball_status.v.x = ball_status.v.y = 0;
		ball_status.a.x = ball_status.a.y = 0;
	}
	if (ball.x > court.width - 1) {
		if (ball.y >= (2 * court.height / 5) && ball.y <= (3 * court.height / 5)) {
			ball.x = court.width / 2;
			ball.y = court.height / 2;
			rscore += 1;
			char tmp[512] = {0};
			sprintf(tmp, "%s get 1 score", ball_status.name);
			score_send(ball_status.name);
			Show_Message( , NULL, tmp, 1);

		} else {
			ball.x = court.width - 2;
		}
		ball_status.v.x = ball_status.v.y = 0;
		ball_status.a.x = ball_status.a.y = 0;
	}
	if (ball.y <= 0) {
		ball.y = 1;
		ball_status.v.x = ball_status.v.y = 0;
		ball_status.a.x = ball_status.a.y = 0;
	}
	if (ball.y >= court.height - 1) {
		ball.y = court.height - 2;
		ball_status.v.x = ball_status.v.y = 0;
		ball_status.a.x = ball_status.a.y = 0;
	}
	w_gotoxy_putc(Football, (int)ball.x, (int)ball.y, 'o');
}

void re_drew_score() {
	w_gotoxy_puts(Score, 1, 1, "BLUE TEAM");
	w_gotoxy_puts(Score, 11, 1, "RED TEAM");
	char tmp[10] = {0}, tmp2[10] = {0};
	sprintf(tmp, "%d", bscore);
	sprintf(tmp2, "%d", rscore);
	w_gotoxy_puts(Score, 3, 3, tmp);
	w_gotoxy_puts(Score, 13, 3, tmp2);

}

void re_drew() {
	werase(Football_t);
	box(Football_t, 0, 0);
	box(Football, 0, 0);
	for (int i = (2 * court.height / 5); i <= (3 * court.height / 5); i++) {
		w_gotoxy_puts(Football_t, 1, i + 1, "x");
		w_gotoxy_puts(Football_t, court.width + 2, i + 1, "x");
	}
	re_drew_team(rteam);
	re_drew_team(bteam);
	re_drew_ball();
	re_drew_score();
	server_send();
	wrefresh(Football);
	wrefresh(Football_t);
}
