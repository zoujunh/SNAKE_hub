#define _CRT_SECURE_NO_WARNINGS 1
#include "snake.h"



void test()
{
	//����̰���� 
	int ch = 0;
	do
	{
		Snake snake = { 0 };
		Gamestart(&snake);//��Ϸ��ʼǰ�ĳ�ʼ��
		Gamerun(&snake);  // ����Ϸ�Ĺ���
		Gameend(&snake);  //�ƺ�Ĺ���
		setpos(20, 15);
		printf("����һ����(Y/N):");
		ch = getchar();
		getchar();  // ����\n
	} while (ch == 'Y' || ch == 'y');


}


int main()
{
	//�޸����䱾�����Ļ���
	setlocale(LC_ALL, "");

	test();//��Ϸ����
	setpos(0, 27);
	return 0;
}