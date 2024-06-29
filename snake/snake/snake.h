#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>

#define WALL  L'■'
#define BODY  L'●'
#define FOOD  L'★'

//蛇默认的起始位置
#define POS_X 24
#define POS_Y 5

#define KEY_PRESS(VK) ( (GetAsyncKeyState(VK) & 0x1) ? 1 : 0 )


//游戏状态
enum GAME_STATUS  //枚举
{
	ok=1, //正常运行
	ESC,  //按esc健退出
	kill_by_wall, //撞墙
	kill_by_self //撞到自己了

};

enum DIRECTION
{
	up=1,
	down,
	left,
	right
};

//蛇身结点的定义
typedef struct SnakeNode
{
	int x;
	int y;
	struct SnakeNode* next;
}SnakeNode, * psnakenode;


//贪吃蛇-整个游戏的维护
typedef struct Snake
{
	psnakenode psnake;//指针定义变量，维护整条蛇的指针  是指向蛇头的
	psnakenode pfood;//指向食物的指针
	int score;//当前累计分数
	int foodweight;//一个食物的分数
	int sleepTime;//蛇身休眠的时间，休眠的时间越短，速度越快，休眠时间越长，速度越慢
	enum GAME_STATUS status;//游戏当前的状态
	enum DIRECTION dir;//蛇当前走的方向

}Snake, * psnake;


//定位控制台的光标位置
void setpos(int x, int y);

//准备环节
void Gamestart(psnake ps);

//欢迎界面
void welcometogame();

//绘制地图
void Createmap();

//初始化贪吃蛇
void initsnake(psnake ps);

//创建食物
void createfood(psnake ps);

//游戏运行的整个逻辑
void Gamerun(psnake ps);

//打印帮助信息
void printhelp();

//蛇移动的函数
void snakemove(psnake ps);

//判断蛇头的下一步要走的位置处是否为食物
int nextisfood(psnake ps, psnakenode pnext);


//下一步是食物，吃掉去
void eatfood(psnake ps, psnakenode pnext);

//下一步不是食物，不吃掉
void noteatfood(psnake ps, psnakenode pnext);

//检测是否撞墙
void killbywall(psnake ps);

//检测是否撞到自己
void killbyself(psnake ps);

//游戏结束的资源释放
void Gameend(psnake ps);