#include "head.h"

extern struct Map court;
extern WINDOW *Football, *Message, *Help, *Score, *Write, *Football_t;
int message_num = 0;
int rscore = 0;
int bscore = 0;

WINDOW *create_newwin(int width, int height, int startx, int starty) {
    WINDOW *win;
    win = newwin(height, width, starty, startx);
    box(win, 0, 0);
    wrefresh(win);
    //move(LINES - 1, 1);
    return win;
}

void destroy_win(WINDOW *win) {
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(win);
    delwin(win);
    return ;
}

void gotoxy(int x, int y) {
    move(y, x);
}

void gotoxy_putc(int x, int y, int c) {
    move(y, x);
    addch(c);
    move(LINES - 1, 1);
    refresh();
}

void gotoxy_puts(int x, int y, char *s) {
    move(y, x);
    addstr(s);
    move(LINES - 1, 1);
    refresh();
}

void w_gotoxy_putc(WINDOW *win, int x, int y, int c) {
    mvwaddch(win, y, x, c);
    move(LINES - 1, 1);
    wrefresh(win);
    return ;
}

void w_gotoxy_puts(WINDOW *win, int x, int y, char *s) {
    //mvwaddstr(win, y, x, s);
    mvwprintw(win, y, x, s);
    move(LINES - 1, 1);
    wrefresh(win);
    return ;
}

void initfootball() {
    initscr();
    clear();
    if (!has_colors() || start_color() == ERR) {
        endwin();
        fprintf(stderr, "");
        exit(1);
    }
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    init_pair(6, COLOR_BLUE, COLOR_BLACK);
    init_pair(7, COLOR_BLUE, COLOR_GREEN);
    init_pair(8, COLOR_BLUE, COLOR_RED);
    init_pair(9, COLOR_BLUE, COLOR_BLUE);
    init_pair(10, COLOR_BLUE, COLOR_YELLOW);
    init_pair(11, COLOR_BLUE, COLOR_WHITE);
    init_pair(12, COLOR_BLUE, COLOR_CYAN);

    Football_t = create_newwin(court.width + 4, court.height + 2, court.start.x - 2, court.start.y - 1);
    Football = subwin(Football_t, court.height, court.width, court.start.x, court.start.y);
	box(Football, 0, 0);
	wrefresh(Football);
	WINDOW *Message_t = create_newwin(court.width + 4, 7, court.start.x - 2, court.start.y + court.height + 1);
    Message = subwin(Message_t, 5, court.width + 2, court.start.y + court.height + 2, court.start.x - 1);
	scrollok(Message, 1);
	Help = create_newwin(20, court.height + 2, court.start.x + court.width + 2, court.start.y - 1);
    Score = create_newwin(20, 7, court.start.x + court.width + 2, court.start.y + court.height + 1);
    Write = create_newwin(court.width + 24, 5, court.start.x - 2, court.start.y + court.height + 8);
	
	for (int i = (2 * court.height / 5); i <= (3 * court.height / 5); i++) {
		w_gotoxy_puts(Football_t, 1, i + 1, "x");
		w_gotoxy_puts(Football_t, court.width + 2, i + 1, "x");
	}

    return ;
}

void init_help() {
	w_gotoxy_puts(Help, 1, 1, "wasd - 控制上下左右");
	w_gotoxy_puts(Help, 1, 2, "  j  - 停球");
	w_gotoxy_puts(Help, 1, 3, "  k  - 踢球");
	w_gotoxy_puts(Help, 1, 4, "  l  - 带球");
	w_gotoxy_puts(Help, 1, 5, "空格 - 选择力度");
}

void *draw(void *arg) {
    initfootball();
	init_help();
    while (1) {
        sleep(10);
    }
    return NULL;
}

void show_message(WINDOW *win, struct User *user, char *msg, int type) {
	time_t time_now = time(NULL);
	struct tm* tm = localtime(&time_now);
	char timestr[20] = {0};
	char username[80] = {0};
	sprintf(timestr, "%02d:%02d:%02d ", tm->tm_hour, tm->tm_min, tm->tm_sec);
	if (type) { //1系统信息
		wattron(win, COLOR_PAIR(4));
		strcpy(username, "Server Info : ");
	} else {
		if (user->team) {
			wattron(win, COLOR_PAIR(6));
		} else {
			wattron(win, COLOR_PAIR(2));
		}
		sprintf(username, "%s : ", user->name);
	}
	if (message_num < 4) {
		w_gotoxy_puts(win, 10, message_num, username);
		wattron(win, COLOR_PAIR(3));
		w_gotoxy_puts(win, 10 + strlen(username), message_num, msg);
		wattron(win, COLOR_PAIR(5));
		w_gotoxy_puts(win, 1, message_num, timestr);
		message_num += 1;
	} else {
		message_num = 4;
		scroll(win);
		w_gotoxy_puts(win, 10, message_num, username);
		wattron(win, COLOR_PAIR(3));
		w_gotoxy_puts(win, 10 + strlen(username), message_num, msg);
		wattron(win, COLOR_PAIR(5));
		w_gotoxy_puts(win, 1, message_num, timestr);
	}
	wrefresh(win);
}
