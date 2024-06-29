#define _CRT_SECURE_NO_WARNINGS 1
#include "snake.h"


void setpos(int x, int y)//����
{
	//����豸���
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	//���ݾ�����ù���λ��
	COORD pos = { x, y };
	SetConsoleCursorPosition(handle, pos);
}


void welcometogame()
{
	//��ӭ��Ϣ
	setpos(40, 10);
	printf("��ӭ����̰����С��Ϸ\n");
	setpos(42, 20);
	system("pause");
	system("cls");

	//������Ϣ
	setpos(25, 10);
	printf("�á� �� �� �� �������ߵ��ƶ���F3�Ǽ��٣�F4�Ǽ���\n");
	setpos(25, 11);
	printf("�����ܵõ����ߵķ���");
	setpos(42, 20);
	system("pause");
	system("cls");
}


void Createmap()
{
	int i = 0;
	//��
	setpos(0, 0);
	for (i = 0;i <= 56;i += 2)
	{
		wprintf(L"%lc", WALL);
	}
	//��
	setpos(0, 26);
	for (i = 0;i <= 56;i += 2)
	{
		wprintf(L"%lc", WALL);
	}

	//��
	for (i = 1;i <= 25;i++)
	{
		setpos(0, i);
		wprintf(L"%lc", WALL);
	}
	//��
	for (i = 1;i <= 25;i++)
	{
		setpos(56, i);
		wprintf(L"%lc", WALL);
	}
}


void initsnake(psnake ps)
{
	//�����������Ľ��
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

		//ͷ��
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
	//��ӡ����
	cur = ps->psnake;
	while (cur)
	{
		setpos(cur->x, cur->y);
		wprintf(L"%lc", BODY);
		cur = cur->next;
	}

	//̰���ߵ�������Ϣ��ʼ��
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

	//������ߵ�ÿ�������������Ƚ�
	psnakenode cur = ps->psnake;
	while (cur)
	{
		if (x == cur->x && y == cur->y)
		{
			goto again;
		}
		cur = cur->next;
	}


	//����ʳ��
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
	//���ÿ���̨����Ϣ
	system("mode con cols=100 lines=30");
	system("title ̰����");


	//���ع��
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);//��ȡ����̨�����Ϣ
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(handle, &CursorInfo);

	//��ӡ��ӭ��Ϣ
	welcometogame();
	//���Ƶ�ͼ
	Createmap();
	//��ʼ����
	initsnake(ps);
	//����ʳ��
	createfood(ps);
}

void printhelp()
{
	setpos(62, 15);
	printf("1.���ܴ�ǽ������ҧ���Լ�");
	setpos(62, 16);
	printf("2.�á� �� �� �� �������ߵ��ƶ�");
	setpos(62, 17);
	printf("3.F3�Ǽ��٣�F4�Ǽ���");
	setpos(62, 18);
	printf("4.Esc: �˳���Ϸ  space: ��ͣ��Ϸ");
	setpos(62, 19);
	printf("��Ȩ@zjh");

	
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
		return 1;//��һ��������ʳ��
	else
		return 0;
}


void eatfood(psnake ps, psnakenode pnext)
{
	pnext->next = ps->psnake;
	ps->psnake = pnext;

	psnakenode cur = ps->psnake;
	//��ӡ����
	while (cur)
	{
		setpos(cur->x, cur->y);
		wprintf(L"%lc", BODY);
		cur = cur->next;
	}
	ps->score += ps->foodweight;

	free(ps->pfood);//����ʳ���ͷŵ�ʳ��Ľ��

	//�½�ʳ��
	createfood(ps);
}


void noteatfood(psnake ps, psnakenode pnext)
{
	pnext->next = ps->psnake;
	ps->psnake = pnext;

	//������ͷ�β�ڵ�
	psnakenode cur = ps->psnake;
	while (cur->next->next != NULL)//ֱ���ҵ�β�ڵ�Ϊֹ
	{
		setpos(cur->x, cur->y);
		wprintf(L"%lc", BODY);
		cur = cur->next;
	}
	//��β�ڵ��λ�ô�ӡ�ɿհ��ַ�
	setpos(cur->next->x, cur->next->y);
	printf("  ");

	free(cur->next); //�ͷŵ����һ���ڵ�
	cur->next = NULL;

}


//����Ƿ�ײǽ
void killbywall(psnake ps)
{
	if (ps->psnake->x == 0 || ps->psnake->x == 56 ||
		ps->psnake->y == 0 || ps->psnake->y == 26)
	{
		ps->status = kill_by_wall;
	}
}



//����Ƿ�ײ���Լ�
void killbyself(psnake ps)
{
	psnakenode cur = ps->psnake->next;//�ӵڶ����ڵ㿪ʼ
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

	//��һ�������Ƿ�Ϊʳ��
	if (nextisfood(ps, pnext))
	{
		//��ʳ��ͳԵ�
		eatfood(ps, pnext);
	}
	else
	{
		//����ʳ���������
		noteatfood(ps, pnext);
	}

	//����Ƿ�ײǽ
	killbywall(ps);

	//���ײ���Լ�
	killbyself(ps);
}



void Gamerun(psnake ps)
{
	//��ӡ������Ϣ
	printhelp();

	do
	{
		//��ӡ��ǰ�ķ������
		setpos(62, 10);
		printf("�ܷ֣�%5d\n", ps->score);
		setpos(62, 11);
		printf("ʳ��ķ�ֵ��%02d\n", ps->foodweight);
		//��ⰴ����������������
		//�ϡ��¡����ҡ�Esc���ո�F3��F4
		if (KEY_PRESS(VK_UP) && ps->dir != down) //����Ҫ�����ߣ��ߵ�ǰ�ķ���������
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
			//��Ϸ��ͣ
			pause();//��Ϸ��ͣ�ͻظ���ͣ
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

		//��һ��
		snakemove(ps);

		//˯��һ��
		Sleep(ps->sleepTime);


	} while (ps->status == ok);


}



void Gameend(psnake ps)
{
	setpos(15, 12);
	switch (ps->status)
	{
	case ESC:
		printf("�����˳���Ϸ\n");
		break;
	case kill_by_wall:
		printf("���ź���ײǽ�ˣ���Ϸ����\n");
		break;
	case kill_by_self:
		printf("���ź���ҧ���Լ��ˣ���Ϸ����\n");
		break;
	}
	//�ͷ�̰���ߵ�������Դ
	psnakenode cur = ps->psnake;
	psnakenode del = NULL;

	while (cur)
	{
		del = cur;
		cur = cur->next;
		free(del); //����delָ���ű������Ľڵ�cur����ȥ�ͷ�del
	}
	free(ps->pfood);
	ps = NULL;
}