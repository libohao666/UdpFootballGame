/*************************************************************************
    > File Name: game.h
    > Author: 冲鸭！小星星
    > Mail: N0 Mail! 
    > Created Time: 2020年06月06日 星期六 22时09分04秒
 ************************************************************************/

#ifndef _GAME_H
#define _GAME_H
#include <curses.h>

#define MAX 50


WINDOW *Football, *Message, *Help, *Score, *Write;

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

    Football = create_newwin(court.width, court.height, court.start.x, court.start.y);
    Message = create_newwin(court.width, 5, court.start.x, court.start.y + court.height);
    Help = create_newwin(20, court.height, court.start.x + court.width, court.start.y);
    Score = create_newwin(20, 5, court.start.x + court.width, court.start.y + court.height);
    Write = create_newwin(court.width + 20, 5, court.start.x, court.start.y + court.height + 5);
    return ;
}

void *draw(void *arg) {
    initfootball();
    while (1) {
        sleep(10);
    }
    return NULL;
}


#endif
