#define _CRT_SECURE_NO_WARNINGS 1
#include "snake.h"


void setpos(int x, int y)//坐标
{
	//获得设备句柄
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	//根据句柄设置光标的位置
	COORD pos = { x, y };
	SetConsoleCursorPosition(handle, pos);
}


void welcometogame()
{
	//欢迎信息
	setpos(40, 10);
	printf("欢迎来到贪吃蛇小游戏\n");
	setpos(42, 20);
	system("pause");
	system("cls");

	//功能信息
	setpos(25, 10);
	printf("用↑ ↓ ← → 来控制蛇的移动，F3是加速，F4是减速\n");
	setpos(25, 11);
	printf("加速能得到更高的分数");
	setpos(42, 20);
	system("pause");
	system("cls");
}


void Createmap()
{
	int i = 0;
	//上
	setpos(0, 0);
	for (i = 0;i <= 56;i += 2)
	{
		wprintf(L"%lc", WALL);
	}
	//下
	setpos(0, 26);
	for (i = 0;i <= 56;i += 2)
	{
		wprintf(L"%lc", WALL);
	}

	//左
	for (i = 1;i <= 25;i++)
	{
		setpos(0, i);
		wprintf(L"%lc", WALL);
	}
	//右
	for (i = 1;i <= 25;i++)
	{
		setpos(56, i);
		wprintf(L"%lc", WALL);
	}
}


void initsnake(psnake ps)
{
	//创建五个蛇身的结点
	psnakenode cur = NULL;
	int i = 0;
	for (i = 0;i < 5;i++)
	{
		cur = (psnakenode)malloc(sizeof(SnakeNode));
		if (cur == NULL)
		{
			perror("initsnake():malloc");
			return;
		}
		cur->x = POS_X + 2 * i;
		cur->y = POS_Y;
		cur->next = NULL;

		//头插
		if (ps->psnake == NULL)
		{
			ps->psnake = cur;
		}
		else
		{
			cur->next = ps->psnake;
			ps->psnake = cur;
		}
	}
	//打印蛇身
	cur = ps->psnake;
	while (cur)
	{
		setpos(cur->x, cur->y);
		wprintf(L"%lc", BODY);
		cur = cur->next;
	}

	//贪吃蛇的其他信息初始化
	ps->dir = right;
	ps->foodweight = 10;
	ps->pfood = NULL;
	ps->score = 0;
	ps->sleepTime = 200;
	ps->status = ok;
}



void createfood(psnake ps)
{
	int x = 0;
	int y = 0;

again:
	do
	{
		x = rand() % 53 + 2;
		y = rand() % 24 + 1;
	} while (x % 2 != 0);

	//坐标和蛇的每个身体结点的坐标比较
	psnakenode cur = ps->psnake;
	while (cur)
	{
		if (x == cur->x && y == cur->y)
		{
			goto again;
		}
		cur = cur->next;
	}


	//创建食物
	psnakenode pfood = (psnakenode)malloc(sizeof(SnakeNode));
	if (pfood == NULL)
	{
		perror("createfood():malloc()");
		return;
	}

	pfood->x = x;
	pfood->y = y;

	ps->pfood = pfood;
	setpos(x, y);
	wprintf(L"%lc", FOOD);
	
}



void Gamestart(psnake ps)
{
	//设置控制台的信息
	system("mode con cols=100 lines=30");
	system("title 贪吃蛇");


	//隐藏光标
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);//获取控制台光标信息
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(handle, &CursorInfo);

	//打印欢迎信息
	welcometogame();
	//绘制地图
	Createmap();
	//初始化蛇
	initsnake(ps);
	//创建食物
	createfood(ps);
}

void printhelp()
{
	setpos(62, 15);
	printf("1.不能穿墙，不能咬到自己");
	setpos(62, 16);
	printf("2.用↑ ↓ ← → 来控制蛇的移动");
	setpos(62, 17);
	printf("3.F3是加速，F4是减速");
	setpos(62, 18);
	printf("4.Esc: 退出游戏  space: 暂停游戏");
	setpos(62, 19);
	printf("版权@zjh");

	
}


void pause()
{
	while (1)
	{
		Sleep(100);
		if (KEY_PRESS(VK_SPACE))
		{
			break;
		}
	}
}


int nextisfood(psnake ps, psnakenode pnext)
{
	if (ps->pfood->x == pnext->x && ps->pfood->y == pnext->y)
		return 1;//下一个坐标是食物
	else
		return 0;
}


void eatfood(psnake ps, psnakenode pnext)
{
	pnext->next = ps->psnake;
	ps->psnake = pnext;

	psnakenode cur = ps->psnake;
	//打印蛇身
	while (cur)
	{
		setpos(cur->x, cur->y);
		wprintf(L"%lc", BODY);
		cur = cur->next;
	}
	ps->score += ps->foodweight;

	free(ps->pfood);//吃完食物释放掉食物的结点

	//新建食物
	createfood(ps);
}


void noteatfood(psnake ps, psnakenode pnext)
{
	pnext->next = ps->psnake;
	ps->psnake = pnext;

	//插完后释放尾节点
	psnakenode cur = ps->psnake;
	while (cur->next->next != NULL)//直到找到尾节点为止
	{
		setpos(cur->x, cur->y);
		wprintf(L"%lc", BODY);
		cur = cur->next;
	}
	//将尾节点的位置打印成空白字符
	setpos(cur->next->x, cur->next->y);
	printf("  ");

	free(cur->next); //释放掉最后一个节点
	cur->next = NULL;

}


//检测是否撞墙
void killbywall(psnake ps)
{
	if (ps->psnake->x == 0 || ps->psnake->x == 56 ||
		ps->psnake->y == 0 || ps->psnake->y == 26)
	{
		ps->status = kill_by_wall;
	}
}



//检测是否撞到自己
void killbyself(psnake ps)
{
	psnakenode cur = ps->psnake->next;//从第二个节点开始
	while (cur)
	{
		if (cur->x == ps->psnake->x && cur->y == ps->psnake->y)
		{
			ps->status = kill_by_self;
			return;
		}
		cur = cur->next;
	}

}



void snakemove(psnake ps)
{
	psnakenode pnext = (psnakenode)malloc(sizeof(SnakeNode));
	if (pnext == NULL)
	{
		perror("snakemove()::malloc()");
		return;
	}
	pnext->next = NULL;

	switch (ps->dir)
	{
	case up:
		pnext->x = ps->psnake->x;
		pnext->y = ps->psnake->y - 1;
		break;
	case down:
		pnext->x = ps->psnake->x;
		pnext->y = ps->psnake->y + 1;
		break;
	case left:
		pnext->x = ps->psnake->x - 2;
		pnext->y = ps->psnake->y;
		break;
	case right:
		pnext->x = ps->psnake->x + 2;
		pnext->y = ps->psnake->y;
		break;
	}

	//下一个坐标是否为食物
	if (nextisfood(ps, pnext))
	{
		//是食物就吃掉
		eatfood(ps, pnext);
	}
	else
	{
		//不是食物就正常走
		noteatfood(ps, pnext);
	}

	//检测是否撞墙
	killbywall(ps);

	//检测撞到自己
	killbyself(ps);
}



void Gamerun(psnake ps)
{
	//打印帮助信息
	printhelp();

	do
	{
		//打印当前的分数情况
		setpos(62, 10);
		printf("总分：%5d\n", ps->score);
		setpos(62, 11);
		printf("食物的分值：%02d\n", ps->foodweight);
		//检测按键，看看往哪里走
		//上、下、左、右、Esc、空格、F3、F4
		if (KEY_PRESS(VK_UP) && ps->dir != down) //按键要往上走，蛇当前的方向不能是下
		{
			ps->dir = up;
		}
		else if (KEY_PRESS(VK_DOWN) && ps->dir != up)
		{
			ps->dir = down;
		}
		else if (KEY_PRESS(VK_LEFT) && ps->dir != right)
		{
			ps->dir = left;
		}
		else if (KEY_PRESS(VK_RIGHT) && ps->dir != left)
		{
			ps->dir = right;
		}
		else if (KEY_PRESS(VK_ESCAPE))
		{
			ps->status = ESC;
			break;
		}
		else if (KEY_PRESS(VK_SPACE))
		{
			//游戏暂停
			pause();//游戏暂停和回复暂停
		}
		else if (KEY_PRESS(VK_F3))
		{
			if (ps->sleepTime >= 80)
			{
				ps->sleepTime -= 30;
				ps->foodweight += 2;
			}
		}
		else if (KEY_PRESS(VK_F4))
		{
			if (ps->foodweight >= 2)
			{
				ps->sleepTime += 30;
				ps->foodweight -= 2;
			}
		}

		//走一步
		snakemove(ps);

		//睡眠一下
		Sleep(ps->sleepTime);


	} while (ps->status == ok);


}



void Gameend(psnake ps)
{
	setpos(15, 12);
	switch (ps->status)
	{
	case ESC:
		printf("主动退出游戏\n");
		break;
	case kill_by_wall:
		printf("很遗憾，撞墙了，游戏结束\n");
		break;
	case kill_by_self:
		printf("很遗憾，咬到自己了，游戏结束\n");
		break;
	}
	//释放贪吃蛇的链表资源
	psnakenode cur = ps->psnake;
	psnakenode del = NULL;

	while (cur)
	{
		del = cur;
		cur = cur->next;
		free(del); //先用del指针存放遍历到的节点cur，再去释放del
	}
	free(ps->pfood);
	ps = NULL;
}