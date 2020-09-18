#include "head.h"
#include "game.h"
#define MAX 50

extern WINDOW *Football, *Football_t, *Score;
extern struct Bpoint ball;
extern struct Map court;
extern int rscore, bscore;

void re_drew_player(cJSON *users) {
	int user_cnt = cJSON_GetArraySize(users);
	for (int i = 0; i < user_cnt; i++) {
		cJSON *user = cJSON_GetArrayItem(users, i);
		int team = cJSON_GetObjectItem(user, "team")->valueint;
		int locx = cJSON_GetObjectItem(user, "locx")->valueint;
		int locy = cJSON_GetObjectItem(user, "locy")->valueint;
		
		char p = 'K';
		wattron(Football, COLOR_PAIR((team ? 6 : 2)));
		w_gotoxy_putc(Football, locx, locy, p);
		w_gotoxy_puts(Football, locx + 1, locy - 1, cJSON_GetObjectItem(user, "name")->valuestring);
		wattroff(Football, COLOR_PAIR((team ? 6 : 2)));
	}
}

void re_drew_ball(cJSON *c_ball) {
	int ballx = cJSON_GetObjectItem(c_ball, "x")->valueint;
	int bally = cJSON_GetObjectItem(c_ball, "y")->valueint;
	w_gotoxy_putc(Football, ballx, bally, 'o');
}

void re_drew_score(cJSON *c_score) {
	int bscore = cJSON_GetObjectItem(c_score, "bscore")->valueint;
	int rscore = cJSON_GetObjectItem(c_score, "rscore")->valueint;
	w_gotoxy_puts(Score, 1, 1, "BLUE TEAM");
	w_gotoxy_puts(Score, 11, 1, "RED TEAM");
	char tmp[10] = {0}, tmp2[10] = {0};
	sprintf(tmp, "%d", bscore);
	sprintf(tmp2, "%d", rscore);
	w_gotoxy_puts(Score, 3, 3, tmp);
	w_gotoxy_puts(Score, 13, 3, tmp2);
}

void re_drew(cJSON *root) {
	cJSON *users = cJSON_GetObjectItem(root, "users");
	cJSON *c_ball = cJSON_GetObjectItem(root, "ball");
	cJSON *c_score = cJSON_GetObjectItem(root, "score");

	werase(Football_t);
	box(Football_t, 0, 0);
	box(Football, 0, 0);
	for (int i = (2 * court.height / 5); i <= (3 * court.height / 5); i++) {
		w_gotoxy_puts(Football_t, 1, i + 1, "x");
		w_gotoxy_puts(Football_t, court.width + 2, i + 1, "x");
	}

	re_drew_player(users);
	re_drew_ball(c_ball);
	re_drew_score(c_score);

	wrefresh(Football);
	wrefresh(Football_t);
}
