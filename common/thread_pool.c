#include "head.h"
#include "thread_pool.h"
#include "udp_epoll.h"
#include "game.h"
#include "show_data_stream.h"
#include "ball_status.h"

extern int bepollfd, repollfd;
extern struct BallStatus ball_status;

void do_echo(struct User *user) {
	struct FootBallMsg msg;
	int size = recv(user->fd, (void *)&msg, sizeof(msg), 0);
	user->flag = 10;
	if (msg.type & FT_ACK) {
		if (user->team)
			DBG(L_BLUE" %s "NONE"心跳\n", user->name);
		else 
			DBG(L_RED" %s "NONE"心跳\n", user->name);
	}else if (msg.type & (FT_WALL | FT_MSG)) {
		if (user->team)
			DBG(L_BLUE" %s : %s\n"NONE, user->name, msg.msg);
		else 
			DBG(L_RED" %s : %s\n"NONE, user->name, msg.msg);
		strcpy(msg.name, user->name);
		msg.team = user->team;
		Show_Message( , user, msg.msg, );
		send(user->fd, (void *)&msg, sizeof(msg), 0);
	} else if (msg.type & FT_FIN) {
		show_data_stream('e');
		pthread_mutex_lock(&ball_status.mutex);
		if (user == ball_status.user) ball_status.carry = 0;
		pthread_mutex_unlock(&ball_status.mutex);
		DBG(RED"%s logout.\n", user->name);
		char tmp[512] = {0};
		sprintf(tmp, "%s Logout!", user->name);
		Show_Message( , NULL, tmp, 1);
		user->online = 0;
		int epollfd_tmp = (user->team ? bepollfd : repollfd);
		del_event(epollfd_tmp, user->fd);
	} else if (msg.type & FT_CTL) {
		show_data_stream('n');
		if (msg.ctl.dirx || msg.ctl.diry) {
			user->loc.x += msg.ctl.dirx;
			user->loc.y += msg.ctl.diry;
			if (user->loc.x <= 0) user->loc.x = 0;
			if (user->loc.x >= court.width - 1) user->loc.x = court.width - 1;
			if (user->loc.y <= 0) user->loc.y = 0;
			if (user->loc.y >= court.height - 1) user->loc.y = court.height - 1;
		}
		pthread_mutex_lock(&ball_status.mutex);
		if (msg.ctl.action & ACTION_KICK) {
			show_data_stream('k');
			if (can_kick(&user->loc, msg.ctl.strength)) {
				ball_status.carry = 0;
				ball_status.who = user->team;
				strcpy(ball_status.name, user->name);
			}
		} else if (msg.ctl.action & ACTION_STOP) {
			show_data_stream('s');
			if (can_access(&user->loc)) {
				ball_status.carry = 0;
				strcpy(ball_status.name, user->name);
				ball_status.v.x = ball_status.v.y = 0;
				ball_status.a.x = ball_status.a.y = 0;
			}
		} else if (msg.ctl.action & ACTION_CARRY){
			show_data_stream('c');
			if (can_access(&user->loc)) {
				strcpy(ball_status.name, user->name);
				ball_status.carry = 1;
				ball_status.user = user;
				ball_status.v.x = ball_status.v.y = 0;
				ball_status.a.x = ball_status.a.y = 0;
			}
		}
		pthread_mutex_unlock(&ball_status.mutex);
	}
}

void task_queue_init(struct task_queue *taskQueue, int sum, int epollfd) {
	taskQueue->sum = sum;
	taskQueue->epollfd = epollfd;
	taskQueue->team = calloc(sum, sizeof(void *));
	taskQueue->head = taskQueue->tail = 0;
	pthread_mutex_init(&taskQueue->mutex, NULL);
	pthread_cond_init(&taskQueue->cond, NULL);
}

void task_queue_push(struct task_queue *taskQueue, struct User *user) {
	pthread_mutex_lock(&taskQueue->mutex);
	taskQueue->team[taskQueue->tail] = user;
	DBG(L_GREEN"Thread Pool : "NONE" Task Push %s \n", user->name);
	if (++taskQueue->tail == taskQueue->sum) {
		DBG(L_GREEN"Thread Pool : "NONE" Task Queue End.\n");
		taskQueue->tail = 0;
	}
	pthread_cond_signal(&taskQueue->cond);
	pthread_mutex_unlock(&taskQueue->mutex);
}

struct User *task_queue_pop(struct task_queue *taskQueue) {
	pthread_mutex_lock(&taskQueue->mutex);
	while (taskQueue->tail == taskQueue->head) {
		DBG(L_GREEN"Thread Pool : "NONE" Task Queue Empty, Waiting for Task.\n");
		pthread_cond_wait(&taskQueue->cond, &taskQueue->mutex);
	}
	struct User *user = taskQueue->team[taskQueue->head];
	DBG(L_GREEN"Thread Pool : "NONE" Task Pop %s.\n", user->name);
	if (++taskQueue->head == taskQueue->sum) {
		DBG(L_GREEN"Thread Pool : "NONE" Task Queue End.\n");
		taskQueue->head = 0;

	}
	pthread_mutex_unlock(&taskQueue->mutex);
	return user;
}

void *thread_run(void *arg) {
	pthread_t tid = pthread_self();
	pthread_detach(tid);
	struct task_queue *taskQueue = (struct task_queue *)arg;
	while (1) {
		struct User *user = task_queue_pop(taskQueue);
		do_echo(user);
	}
}
