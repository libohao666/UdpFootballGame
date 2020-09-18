#ifndef _DATATYPE_H
#define _DATATYPE_H
#define MAXMSG 512
struct LogRequest {
    char name[80];
    int team; // 0 r 1 b
	char msg[512];
};

struct LogResponse{
	int type; //0 success 1 failed
	char msg[512];
};

struct Point {//位置
    int x, y;;
};

struct Bpoint {
	double x, y;
};

struct User {
    int team;//默认蓝队
	int fd;
	int online;
    char name[80];//队名
    int flag; // 未响应的次数
    struct Point loc; //位置
};

struct Map {//场地
    int width; //宽
	int height;//长
    struct Point start;//开始的点
    int gate_width;
	int gate_height;//球门宽度和高度
};

struct Map court;

#define ACTION_KICK 0x01
#define ACTION_CARRY 0x02
#define ACTION_STOP 0x04

struct Ctrl {
	int action;
	int strength;
	int dirx;
	int diry;
};

#define FT_TEST 0x01
#define FT_WALL 0x02
#define FT_MSG 0x04
#define FT_ACK 0x08
#define FT_FIN 0x10
#define FT_CTL 0x20
#define FT_GAME 0x40
#define FT_SCORE 0x80
#define FT_GAMEOVER 0x100

struct FootBallMsg {
    int type;
    int size;
    int team;
    char name[20];
    struct Ctrl ctl;
    char msg[MAXMSG];
};

struct Aspeed{
    double x;
    double y;
};

struct Speed{
    double x;
    double y;
};


struct BallStatus {
    struct Speed v;
    struct Aspeed a;
    int t;
    int who;
	char name[20];
	struct User *user;
	int carry;
    pthread_mutex_t mutex;
};
#endif
