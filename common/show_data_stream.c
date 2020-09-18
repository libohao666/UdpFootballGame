#include "head.h"

extern char data_stream[20];
extern WINDOW *Help;
extern struct Map court;

void show_data_stream(int type) {
	for (int i = 18; i > 1; i--) {
		data_stream[i] = data_stream[i - 1];
	}
	data_stream[1] = type;
	for (int i = 1; i < 19; i++) {
		switch(data_stream[i]) {
			case 'l':
				wattron(Help, COLOR_PAIR(8));
				break;
			case 'c':
				wattron(Help, COLOR_PAIR(7));
				break;
			case 'k':
				wattron(Help, COLOR_PAIR(8));
				break;
			case 's':
				wattron(Help, COLOR_PAIR(9));
				break;
			case 'n':
				wattron(Help, COLOR_PAIR(10));
				break;
			case 'e':
				wattron(Help, COLOR_PAIR(11));
				break;
			default:
				break;
		}
		mvwprintw(Help, court.height - 1, i, " ");
		wattron(Help, COLOR_PAIR(2));
	}
	wrefresh(Help);
}

