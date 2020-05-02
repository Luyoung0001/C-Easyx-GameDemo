/******************************************************************************

��Ȩ���� (C), 2020-2021, DeRoy

******************************************************************************
�� �� ��   : main.c
�� �� ��   : ����
��    ��   : DeRoy��΢�Ź��ںš����ѧϰ���ء���
��������   : 2020��4��19��
����޸�   :
��������   : �ɻ���ս
�����б�   :
*
*			1.void loadimage();	//����ͼƬ
*			2.void Game_Paint();	//��Ϸ����
*			3.void Game_Init();	//��Ϸ��ʼ��
*			4.void Game_Updata();	//��Ϸ����

*			5.void BackMove();	//�������ƶ�
*			6.void EnemyMove();	//�з��ɻ����ƶ�
*			7.void PlaneMove();	//�ҷ��ɻ����ƶ�
*			8.void BulletMove();	//�ӵ����ƶ�

*			9.void AddEnemy();	//��ӵз��ɻ�
*			10.void FireBullet();	//�����ӵ�
*

�޸���ʷ   :
1.��    ��   : 2020��4��19��
��    ��   : DeRoy
�޸�����   : �����ļ�

******************************************************************************/
/*----------------------------------------------*
* ͷ�ļ�����                                     *
*----------------------------------------------*/
#include<stdio.h>
#include<easyx.h>
#include<time.h>
#include<conio.h>
/*----------------------------------------------*
* �ṹ��                                        *
*----------------------------------------------*/
//�ӵ��Ľṹ�� 
struct bullet
{
	int x;
	int y;
	//�ӵ�����ͨ������ȥ���
	struct bullet* pnext;
};
// �ҷ��ɻ�   
struct plane  //�ҷ��ɻ��Ľṹ��
{
	int x, y;			//�ɻ�������
	bool exist;			//�ɻ��Ƿ���   exist==false(0)  �ɻ����� ;  exist=ture(1) �ɻ�����
	struct bullet* bt;  //�ӵ�
};
//�з��ɻ� 
struct enemy
{
	int x, y; //�з��ɻ�������
	struct enemy* pnext; //����������һ���ɻ��ĵ�ַ
};

/*----------------------------------------------*
* ��������                                     *
*----------------------------------------------*/
void loadimage();	//����ͼƬ
void Game_Paint();	//��Ϸ����
void Game_Init();	//��Ϸ��ʼ��
void Game_Updata();	//��Ϸ����

void BackMove();	//�������ƶ�
void EnemyMove();	//�з��ɻ����ƶ�
void PlaneMove();	//�ҷ��ɻ����ƶ�
void BulletMove();	//�ӵ����ƶ�

void AddEnemy();	//��ӵз��ɻ�
void FireBullet();	//�����ӵ�

/*----------------------------------------------*
* ȫ�ֱ���                                     *
*----------------------------------------------*/
COORD g_bk;			//��������ͼλ��
IMAGE g_enemy[2], g_plane[2], g_Bullet, g_back;	//����ͼƬ
struct plane myPlane;		//�����ҷ��ɻ��Ľṹ�����.
struct enemy* emy_Phead;	//�з��ɻ������ͷ�ڵ�. 
DWORD begin, end;			//��������ʱ������ƶ��ٶ�
DWORD t1, t2;				//��������ʱ����Ƶз��ɻ��Ĳ����ٶ�


int main()
{
	initgraph(512, 768);	/* ��ʼ��ͼ�ο� */
	Game_Init();	/* ��Ϸ�ĳ�ʼ�� */
	while (1)
	{
		end = GetTickCount();	/* ��ȡendʱ�� */
		t2 = GetTickCount();	/* ��ȡt2ʱ�� */
		if (kbhit())			/* �ж��Ƿ��а�����Ϣ */
		{
			PlaneMove();	/* ��Ҳ����ɻ��ƶ��Ϳ��� */
		}
		if (end - begin >= 50)
		{
			Game_Paint();	/* ��Ϸ�Ļ��� */
			Game_Updata();	/* ��Ϸ�ĸ��� */
			begin = end;
		}
		if (t2 - t1 >= 3000)
		{
			AddEnemy();		/* ��ӵз��ɻ� */
			AddEnemy();		/* ��ӵз��ɻ� */
			t1 = t2;
		}
	}
	closegraph();	/* �ر�ͼ�ο� */
	return 0;
}

/* �ӵ��ƶ� */
void BulletMove()
{
	//����ÿһ���ӵ� �����������ƶ�
	struct bullet* pPlane = myPlane.bt;			//ָ���ӵ���ͷ���
	struct bullet* pDelete;						//ָ��Ҫɾ�����ӵ�
	while (pPlane->pnext != NULL)
	{
		//�ӵ����ƶ��ٶ� ����һ����
		pPlane->pnext->y -= 10;

		//�����ӵ�Խ��
		if (pPlane->pnext->y < -50)
		{
			pDelete = pPlane->pnext;
			pPlane->pnext = pDelete->pnext;
			free(pDelete);
			pDelete = NULL;
			continue;
		}
		//�����ӵ�ײ���ɻ�   �ӵ����� ���з��ɻ����Ƚ� 
		// �����з��ɻ� 
		struct enemy *pEnemy = emy_Phead; 
		struct enemy *pDeleteEnemy;  
		//�����з��ɻ���ѭ��
		while (pEnemy->pnext != NULL)
		{
			if ((pPlane->pnext->x >= pEnemy->pnext->x) && pPlane->pnext->x <= (pEnemy->pnext->x + 80) && pPlane->pnext->y <= (pEnemy->pnext->y + 100))
			{
				//�ɻ���ը��ͼƬ ����ȥ 
				//�ѷɻ��ͷŵ�
				pDeleteEnemy = pEnemy->pnext;
				pEnemy->pnext = pDeleteEnemy->pnext;
				free(pDeleteEnemy);
				pDeleteEnemy = NULL;

				// ���ӵ��ͷŵ� 
				pDelete = pPlane->pnext;
				pPlane->pnext = pDelete->pnext;
				free(pDelete);
				pDelete = NULL;
				break;	//���break�Ǹ��ӣ����ӵ����ез�ĳһ�ܷɻ���ʱ���Ѿ�����Ҫ������������ķɻ��ˣ���Ϊ�ӵ��Ѿ�û����
			}
			//�жϵз��ɻ��ǲ���NULL
			if (pEnemy->pnext == NULL)
			{
				break;
			}
			pEnemy = pEnemy->pnext;
		}
		//�ж��ӵ��ǲ���NULL
		if (pPlane->pnext == NULL)
		{
			break;  //����ӵ�û�˾�break
		}
		pPlane = pPlane->pnext;
	}
}

/* �ӵ����� */
void FireBullet()
{
	//�����ӵ���ʵ���Ǵ������.
	struct bullet *newbullet = (struct bullet *)malloc(sizeof(struct bullet));

	//��x��y ��ֵ
	newbullet->x = myPlane.x + 28; //�ӵ���x����		 
	newbullet->y = myPlane.y - 10;  //�ӵ��� y����

	//���ӽ�� 
	newbullet->pnext = myPlane.bt->pnext;
	myPlane.bt->pnext = newbullet;
}

/* �ҷ��ɻ��ƶ� */
void PlaneMove()
{
	// ��ȡ���̰�����Ϣ 
	char ch = getch();  //��ȡ��������
	switch (ch)		// �� �� ��  ��  �����ӵ�
	{
	case 72:		//������    72 �� �� ��ֵ
	case 'W':
	case 'w':
		//�����涨����� ��Ҫ��{}
		myPlane.y -= 20;
		if (myPlane.y < 0)
		{
			myPlane.y = 0;
		}
		break;
	case 80:  //������
	case 'S':
	case 's':
		myPlane.y += 20;
		if (myPlane.y>668)
		{
			myPlane.y = 700;
		}
		break;
	case 75:  //���
	case 'A':
	case 'a':
		myPlane.x -= 20;
		if (myPlane.x < 0)
		{
			myPlane.x = 0;
		}
		break;
	case 77: //�ұ�
	case 'D':
	case 'd':
		myPlane.x += 20;
		if (myPlane.x > 437)
		{
			myPlane.x = 437;
		}
		break;
	case 32: //32�ǿո��
		//���ú���  �����ӵ�
		FireBullet();
		break;
	}
}

/* �����ƶ� */
void BackMove()
{
	g_bk.Y += 2;
	if (g_bk.Y >= 768)
		g_bk.Y = 0;
}

/* �з��ɻ��ƶ� */
void EnemyMove()
{
	struct enemy* pTmp = emy_Phead;//ָ��з��ɻ���ͷ���
	struct enemy* pDelete;

	while (pTmp->pnext != NULL)
	{
		pTmp->pnext->y += rand() % 10; //�ɿ����

		//�жϷɻ��Ƿ�Խ����
		if (pTmp->pnext->y >= 800)
		{
			//��Խ��ķɻ����ٵ�
			pDelete = pTmp->pnext;
			pTmp->pnext = pDelete->pnext;
			free(pDelete);
			pDelete = NULL;
			continue;
		}
		//�������
		pTmp = pTmp->pnext;
	}
}

/* ��ӵз��ɻ� */
void AddEnemy()
{
	//�����½��
	struct enemy* newnode = (struct enemy*)malloc(sizeof(struct enemy));

	newnode->x = rand() % 412;	//x���� ���
	newnode->y = -100;			//y���� �̶�

	//ͷ��
	newnode->pnext = emy_Phead->pnext;
	emy_Phead->pnext = newnode;
}

/* ��Ϸ��ʼ�� */
void Game_Init()
{
	srand((unsigned)time(NULL));						//�����������
	loadimage();
	myPlane.y = 700;			
	myPlane.x = rand() % 500;   //��Χ 0~499 
	myPlane.exist = true;		//�ɻ�����Ϊtrue
	//��ʼ���ӵ������ͷ���
	myPlane.bt = (struct bullet *)malloc(sizeof(struct bullet));
	myPlane.bt->pnext = NULL;

	//��ʼ���з��ɻ���ͷ���
	emy_Phead = (struct enemy *)malloc(sizeof(struct enemy));
	emy_Phead->pnext = NULL;
	//��ʼ����������
	begin = GetTickCount();
	t1 = GetTickCount();
	g_bk.X = 0;
	g_bk.Y = 0;
	//������������ܵз��ɻ�
	AddEnemy();
	AddEnemy();
}

/* ��Ϸ������ */
void Game_Updata()
{
	EnemyMove();	//�ɻ��ƶ�
	BulletMove();	//�ӵ��ƶ�
	BackMove();		//�����ƶ�
}

/* ��Ϸ���� */
void Game_Paint()
{
	//��ʼ������ͼ
	BeginBatchDraw();
	cleardevice();	//ˢ����Ļ
	/*========================������ͼƬ========================*/

	putimage(g_bk.X, g_bk.Y, &g_back);
	putimage(g_bk.X, g_bk.Y - 768, &g_back);

	/*========================���ҷ��ɻ�========================*/
	//�ж��ҷ��ķɻ��Ƿ���,����������ɻ�,���߾�����Ϸʧ�ܵ�ͼƬ.
	if (myPlane.exist)
	{
		putimage(myPlane.x, myPlane.y, &g_plane[1], SRCAND);
		putimage(myPlane.x, myPlane.y, &g_plane[0], SRCPAINT);
	}
	else
	{
		//��Ϸ����
		//�� Game OverͼƬ
	}
	/*========================���з��ɻ�========================*/
	//��������ȥ����
	struct enemy* pTmp = emy_Phead->pnext; //ָ���һ�ܵз��ɻ�
	while (pTmp)
	{
		putimage(pTmp->x, pTmp->y, &g_enemy[1], SRCAND);
		putimage(pTmp->x, pTmp->y, &g_enemy[0], SRCPAINT);
		//Ҫ�������
		pTmp = pTmp->pnext;
	}

	/*========================���ҷ��ɻ��ӵ�========================*/
	struct bullet* pBullet = myPlane.bt->pnext;
	while (pBullet) //��ͼ
	{
		putimage(pBullet->x, pBullet->y, &g_Bullet, SRCPAINT);
		pBullet = pBullet->pnext;
	}
	//����������ͼ
	EndBatchDraw();
}

/* ����ͼƬ */
void loadimage()
{
	//�����ⲿ��ԴͼƬ
	loadimage(&g_back, L"bg_1.bmp");
	loadimage(&g_Bullet, L"Bullet.png", 20, 20);
	loadimage(&g_enemy[0], L"smallenemy.png", 75, 75);
	loadimage(&g_enemy[1], L"smallenemy1.png", 75, 75);
	loadimage(&g_plane[0], L"plane.png", 75, 75);
	loadimage(&g_plane[1], L"plane1.png", 75, 75);
}
