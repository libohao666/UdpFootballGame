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

struct User {
    int team;//默认蓝队
	int fd;
	int online;
    char name[80];//队名
    int flag; // 未响应的次数
    //struct sockaddr_in addr; //远程地址
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

struct Ctrl { //控制
    int carry;//带球
	int kick;//踢球
};

struct TransMsg {//传输的信息
    int dirx;//方向
	int diry;
    struct Ctrl ctrl;
};

#define FT_TEST 0x01
#define FT_WALL 0x02
#define FT_MSG 0x04
#define FT_ACK 0x08
#define FT_FIN 0x10

struct FootBallMsg {
	int type;
	int size;
	int team;
	char name[20];
	char msg[MAXMSG];
};

#endif
