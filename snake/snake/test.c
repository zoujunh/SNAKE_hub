#define _CRT_SECURE_NO_WARNINGS 1
#include "snake.h"



void test()
{
	//创建贪吃蛇 
	int ch = 0;
	do
	{
		Snake snake = { 0 };
		Gamestart(&snake);//游戏开始前的初始化
		Gamerun(&snake);  // 玩游戏的过程
		Gameend(&snake);  //善后的工作
		setpos(20, 15);
		printf("再来一局吗？(Y/N):");
		ch = getchar();
		getchar();  // 清理\n
	} while (ch == 'Y' || ch == 'y');


}


int main()
{
	//修改适配本地中文环境
	setlocale(LC_ALL, "");

	test();//游戏测试
	setpos(0, 27);
	return 0;
}