#include<stdio.h>
#include<easyx.h>	//ͼ�ο�
#include<time.h>	//�����

#define N 4

IMAGE img[4],imgs[16];		//img[4] 4��ԭͼ���ĸ��ؿ� imgs�ָ�ͼƬ
int aim_c, aim_r;		//�հ״�Ŀ������λ��	�� ��
int map[4][4] = { 0 };	//��ͼ��ʼ��Ϊ0
int NUM = 0;			//��һ��ͼƬ������img[0]��

void GameInit();
void DrawMap();
void play();
void judg();

int main()
{
	initgraph(N*100, N*100);
	GameInit();
	while (1)
	{
		play();
		DrawMap();
		judg();
	}
	getchar();
	closegraph();
	return 0;
}

void GameInit()
{
	//������ԴͼƬ
	loadimage(&img[0], L"images/1.jpg", 400, 400);
	loadimage(&img[1], L"images/2.jpg", 400, 400);
	loadimage(&img[2], L"images/3.jpg", 400, 400);
	loadimage(&img[3], L"images/4.jpg", 400, 400);
	// ���û�ͼĿ��Ϊ img ����
	SetWorkingImage(&img[NUM]);
	for (int y = 0, n = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			getimage(&imgs[n++], x * 100, y * 100, (x + 1) * 100, (y + 1) * 100);
		}
	}
	// ���û�ͼĿ��Ϊ��ͼ����
	SetWorkingImage();
	//�������һ��ͼƬΪ�հ�ͼƬ����ΪĿ��ͼƬ
	loadimage(&imgs[15], L"images/15.jpg", 400, 400);
	putimage(0,0,&img[NUM]);

	//�����������
	srand((unsigned)time(NULL));
	//��ʼ����ͼ 0~15
	for (int i = 0, k = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			map[i][j] = k++;
		}
	}
	//���ҵ�ͼ
	for (int i = 0; i <= 100; i++)
	{
		//�õ�Ŀ�������к���
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (map[i][j] == 0)	//��һ�Ųü�ͼƬ��Ϊ����Ŀ��
				{
					aim_r = i;
					aim_c = j;
					break;
				}
			}
		}
		int dir = rand() % 4;		//���һ������
		switch (dir)
		{
		case 0:	//���Ͻ���
			if (aim_r >= 1)
			{
				map[aim_r][aim_c] = map[aim_r - 1][aim_c];
				map[aim_r - 1][aim_c] = 0;
			}
			break;
		case 1:	//���½���
			if (map[aim_r + 1][aim_c] == 15) break;	//���Ϳհ�ͼƬ����
			if (aim_r < 3)
			{
				map[aim_r][aim_c] = map[aim_r + 1][aim_c];
				map[aim_r + 1][aim_c] = 0;
			}
			break;
		case 2:	//���󽻻�
			if (aim_c >= 1)
			{
				map[aim_r][aim_c] = map[aim_r][aim_c - 1];
				map[aim_r][aim_c - 1] = 0;
			}
			break;
		case 3:	//���ҽ���
			if (map[aim_r][aim_c + 1] == 15) break;	//���Ϳհ�ͼƬ����
			if (aim_c < 3)
			{
				map[aim_r][aim_c] = map[aim_r][aim_c + 1];
				map[aim_r][aim_c + 1] = 0;
			}
			break;
		}
	}
	
}

void DrawMap()
{
	BeginBatchDraw();
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			putimage(x * 100, y * 100, &imgs[map[y][x]]);
		}
	}
	EndBatchDraw();
}

void play()
{
	int col, row;			//�������λ�� �� ��
	MOUSEMSG msg;			//�����Ϣ
	msg = GetMouseMsg();	//��ȡ�����Ϣ
	switch (msg.uMsg)		//�������Ϣ����ƥ��
	{
	case WM_LBUTTONDOWN:	//�������Ϣ���������ʱ
		//��ȡ��갴��������
		if (msg.x >= 0 && msg.x < 100)
		{
			col = 0;
		}
		else if (msg.x >= 100 && msg.x < 200)
		{
			col = 1;
		}
		else if (msg.x >= 200 && msg.x < 300)
		{
			col = 2;
		}
		else if (msg.x >= 300 && msg.x <= 500)
		{
			col = 3;
		}

		//��ȡ��갴��������
		if (msg.y >= 0 && msg.y < 100)
		{
			row = 0;
		}
		else if (msg.y >= 100 && msg.y < 200)
		{
			row = 1;
		}
		else if (msg.y >= 200 && msg.y < 300)
		{
			row = 2;
		}
		else if (msg.y >= 300 && msg.y <= 500)
		{
			row = 3;
		}
		//�õ�Ŀ�������к���
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (map[i][j] == 15)	//�հ״�Ϊ����Ŀ��
				{
					aim_r = i;
					aim_c = j;
				}
			}
		}
		//���������λ�ú�Ŀ���Ƿ����ڣ����ڽ�������
		if (row == aim_r && col == aim_c + 1 || 
			row == aim_r && col == aim_c - 1 ||
			row == aim_r + 1 && col == aim_c ||
			row == aim_r - 1 && col == aim_c)
		{
			map[aim_r][aim_c] = map[row][col];
			map[row][col] = 15;
		}
		break;
	}
}

void judg()
{
	if (map[0][0] == 0 && map[0][1] == 1 && map[0][2] == 2 && map[0][3] == 3 && 
		map[1][0] == 4 && map[1][1] == 5 && map[1][2] == 6 && map[1][3] == 7 &&
		map[2][0] == 8 && map[2][1] == 9 && map[2][2] == 10 && map[2][3] == 11 &&
		map[3][0] == 12 && map[3][1] == 13 && map[3][2] == 14 && map[3][3] == 15)
	{
		putimage(0, 0, &img[NUM++]);
		if (NUM == 3)
		{
			MessageBox(NULL, L"��ս�ɹ�", L"Vectory", MB_OK);
		}
		if (MessageBox(NULL, L"�Ƿ������һ��", L"Vectory", MB_YESNO) == IDYES)
		{
			GameInit();	//��Ϸ��ʼ��
		}
		else  exit(0);
	}	
}