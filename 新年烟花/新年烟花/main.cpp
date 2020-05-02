#include<stdio.h>
#include<easyx.h>
#include<conio.h>
#include<math.h>
#include<time.h>
#include <windows.h>
#pragma comment(lib,"winmm.lib") //ʹ�����ֲ��ſ�

#define FLOWER_WIDTH 240
#define FLOWER_HIGHT 240
#define NUM 13

struct FIREWORKS
{
	int m_x, m_y;	//ͼƬ��ͼ�����Ͻ�����
	int cen_x, cen_y;	//�̻����ĵ�����
	int m_r;	//�̻��뾶
	int max_r;	//���뾶
	int m_width, m_hight;	//ͼƬ�ĸ߶ȺͿ��
	int m_Arr[FLOWER_HIGHT][FLOWER_WIDTH];	//����ͼƬ������
	int draw;	//�Ƿ����
	int start;	//�Ƿ�����
	DWORD t1, t2, dt;	//���ŵ��ٶ� GetTickCount()
}flower[NUM];	//�����̻��ṹ�����

struct SHOOT
{
	int m_x, m_y;	//�̻�����x,y����
	int m_hx, m_hy;	//�̻�������ߵ�x,y����
	int shoot;		//�̻���״̬ �Ƿ���
	int height;		//����ľ�������ߴ�
	DWORD t1, t2, dt;	//�����ٶ�
	IMAGE imgs[2];	//һ��һ����֤ͼƬ
	int flag;		//��������״̬�ı�־
}bullet[NUM];

DWORD TimeBegin, TimeNext;	//��ʼʱ�� ��һ��ʱ���
DWORD T1;	//��ʼ���̻���ʱ��
DWORD *pBuff;
MOUSEMSG msg;         //�����Ϣ

void Start();
void CleanPixel();
void LoadImage();
void Init(int i);
void Choose();
void Start_Shoot();
void Show();
void HeartShow();
void MouseMsg();
int main()
{
	srand((unsigned)time(NULL));
	initgraph(1200, 800);

	mciSendString(L"open С����.mp3 alias NewYear", nullptr, 0, nullptr);  //�ļ�·���������·��  alias ȡ����
	mciSendString(_T("play NewYear repeat"), 0, 0, 0);
	Start();
	LoadImage();
	//��ʼ��NUM���ӵ����̻�����
	for (int i = 0; i < NUM; i++)
	{
		Init(i);
	}
	pBuff = GetImageBuffer(NULL);	//��ȡ��ͼ�豸���Դ�ָ��
	T1 = GetTickCount();
	TimeBegin = GetTickCount();

	BeginBatchDraw();
	while (1)
	{
		TimeNext = GetTickCount();
		CleanPixel();
		//ǰ��ʮ��������̻�
		if (TimeNext - TimeBegin <= 20000)
		{
			Choose();
		}
		//��ʮ���뿪ʼȼ�������̻�
		if (TimeNext - TimeBegin == 26000)
		{
			HeartShow();
		}
		//���������·��̻�	Easyx�����GetMouseMsg�е����ܣ�һ��Ҫ��ȡ����Ϣ�ſ��Լ�������Ĵ���
		//if (TimeNext - TimeBegin >= 32000&& TimeNext - TimeBegin <= 42000)
		//{
		//	MouseMsg();
		//}
		if (TimeNext - TimeBegin >= 32000)
		{
			Choose();
		}
		Start_Shoot();
		Show();
		FlushBatchDraw();
	}
	closegraph();
	return 0;
}

void Start()
{
	IMAGE NewYeay;
	loadimage(&NewYeay, L"����.jpg", 1200, 800);
	putimage(0, 0, &NewYeay);
	Sleep(2000);	//��ͣ����
}

void CleanPixel()
{
	Sleep(10);
	for (int clr = 0; clr < 1000; clr++)
	{
		for (int j = 0; j < 2; j++)
		{
			int px1 = rand() % 1200;
			int py1 = rand() % 800;
			if (py1 < 799)	//ʹ�ص���
			{
				pBuff[py1 * 1200 + px1] = pBuff[py1 * 1200 + px1 + 1] = BLACK;
			}
		}
	}
}

void LoadImage()
{
	//����ͼƬ��Դ
	IMAGE flowers, temp;		//�����������ͼƬ��Դ
	loadimage(&flowers, L"flower.jpg");	//����ͼƬ��Դ
	for (int i = 0; i < NUM; i++)
	{
		SetWorkingImage(&flowers);//���û�ͼĿ��Ϊ flowers ����
		getimage(&temp, i * 240, 0, 240, 240);
		SetWorkingImage(&temp);//���û�ͼĿ��Ϊ temp ����

							   //��ͼƬ�е����ش��浽�̻��Ľṹ�����flower[NUM]��ȥ
		for (int y = 0; y < FLOWER_HIGHT; y++)
		{
			for (int x = 0; x < FLOWER_WIDTH; x++)
			{
				flower[i].m_Arr[x][y] = getpixel(x, y);	//��ȡ���ص�
			}
		}
	}

	IMAGE bullets;
	loadimage(&bullets, L"shoot.jpg");	//���ӵ�ȫͼ���ؽ���
	for (int i = 0; i < NUM; i++)
	{
		SetWorkingImage(&bullets);
		int x = rand() % 5;
		getimage(&bullet[i].imgs[0], x * 20, 0, 20, 50);		//��ͼ
		getimage(&bullet[i].imgs[1], (x + 5) * 20, 0, 20, 50);	//��ͼ
	}
	SetWorkingImage();
}

void Init(int i)
{
	//��ʼ���̻��ӵ�����
	int r[13] = { 120, 120, 155, 123, 130, 147, 138, 138, 130, 135, 140, 132, 155 };
	int x[13] = { 120, 120, 110, 117, 110, 93, 102, 102, 110, 105, 100, 108, 110 };
	int y[13] = { 120, 120, 85, 118, 120, 103, 105, 110, 110, 120, 120, 104, 85 };

	//��ʼ���̻�
	flower[i].cen_x = x[i];
	flower[i].cen_y = y[i];
	flower[i].max_r = r[i];
	flower[i].draw = 0;	//�Ƿ�����̻�
	flower[i].m_width = 240;
	flower[i].m_hight = 240;
	flower[i].dt = 5;	//�����ٶ�
	flower[i].m_r = 0;	//��ǰ�뾶Ϊ0 
	flower[i].t1 = GetTickCount();	//��һ�λ�ȡʱ��

	//��ʼ���̻���
	bullet[i].height;
	bullet[i].flag = 0;		//�ӵ�״̬Ϊ��
	bullet[i].shoot = 0;	//������
	bullet[i].dt = rand() % 4 + 4;
	bullet[i].t1 = GetTickCount();	//��һ�λ�ȡʱ��

}

void Choose()
{
	DWORD T2 = GetTickCount();	//t2
	if (T2 - T1 > 100)	//100ms����һ���µ��̻���
	{
		int i = rand() % NUM;	//13���̻����������ȡһ��

		if (bullet[i].shoot == 0 && flower[i].draw == 0)
		{
			bullet[i].m_x = rand() % 1200;
			bullet[i].m_y = rand() % 100 + 600; //600~699
			bullet[i].m_hx = bullet[i].m_x;
			bullet[i].m_hy = rand() % 300;
			bullet[i].shoot = 1;	//����״̬

			putimage(bullet[i].m_x, bullet[i].m_y, &bullet[i].imgs[bullet[i].flag], SRCINVERT);
		}
		T1 = T2;
	}
}

void Start_Shoot()
{
	//��ʼ����
	for (int i = 0; i < NUM; i++)
	{
		bullet[i].t2 = GetTickCount();
		//�ӵ������ƶ�
		if (bullet[i].t2 - bullet[i].t1 >= bullet[i].dt && bullet[i].shoot == 1)
		{
			putimage(bullet[i].m_x, bullet[i].m_y, &bullet[i].imgs[bullet[i].flag], SRCINVERT);
			if (bullet[i].m_y >= bullet[i].m_hy)
			{
				bullet[i].m_y -= 5;
				bullet[i].flag = (++bullet[i].flag) % 2;	//ʹflag��0 1֮���ǻ�
			}
			putimage(bullet[i].m_x, bullet[i].m_y, &bullet[i].imgs[bullet[i].flag], SRCINVERT);
			
			if (bullet[i].m_y <= bullet[i].m_hy)
			{
				putimage(bullet[i].m_x, bullet[i].m_y, &bullet[i].imgs[bullet[i].flag], SRCINVERT);
				bullet[i].shoot = 0;	//ֹͣ����
				flower[i].m_x = bullet[i].m_x;
				flower[i].m_y = bullet[i].m_y;
				flower[i].draw = 1;		//��ʼ����
			}
			bullet[i].t1 = bullet[i].t2;
		}
	}
}

void Show()
{
	int Speed[16] = { 5, 5, 5, 5, 5, 6, 25, 25, 25, 25, 55, 55, 55, 55, 55 };
	for (int i = 0; i < NUM; i++)
	{
		flower[i].t2 = GetTickCount();
		if (flower[i].t2 - flower[i].t1 > flower[i].dt&&flower[i].draw == 1)
		{
			//��ʾ�뾶������
			if (flower[i].m_r < flower[i].max_r)
			{
				flower[i].m_r++;
				flower[i].dt = Speed[flower[i].m_r / 10];
				flower[i].start = 1;
			}


			if (flower[i].m_r >= flower[i].max_r - 1)
			{
				flower[i].start = 0;
				//�̻��������֮�����³�ʼ�����̻����ӵ�����
				Init(i);
			}
			flower[i].t1 = flower[i].t2;
		}

		if (flower[i].start)
		{
			for (double a = 0; a <= 6.28; a += 0.01)				//һȦһȦ�Ļ���
			{
				int x1 = (int)(flower[i].cen_x + flower[i].m_r*cos(a));
				int y1 = (int)(flower[i].cen_y + flower[i].m_r*sin(a));
				if (x1>0 && x1<flower[i].m_width&&y1>0 && y1<flower[i].m_hight)
				{
					int b =  flower[i].m_Arr[x1][y1] & 0xff;		//blue��ɫ������Ӧ�����ص� 
					int g = (flower[i].m_Arr[x1][y1] >> 8) & 0xff;	//green��ɫ������Ӧ�����ص� 
					int r = (flower[i].m_Arr[x1][y1] >> 16);		//red��ɫ    ��Ӧ�Ĵ�С 

					int xx = (int)(flower[i].m_x + flower[i].m_r*cos(a));
					int yy = (int)(flower[i].m_y + flower[i].m_r*sin(a));

					if (r>0x20 && r>0x20 && g>0x20 && xx>0 && xx<1200 && yy>0 && yy<800)	//ȥ����ɫ��
						pBuff[yy * 1200 + xx] = BGR(flower[i].m_Arr[x1][y1]);
				}
			}
			flower[i].start = false;
		}
	}
}

void HeartShow()
{
	int x[NUM] = { 60, 75, 91, 100, 95, 75, 60, 45, 25, 15, 25, 41, 60 };
	int y[NUM] = { 65, 53, 40, 22, 5, 4, 20, 4, 5, 22, 40, 53, 65 };
	for (int i = 0; i < NUM; i++)
	{
		if (bullet[i].shoot == 0 && flower[i].draw == 0)
		{
			bullet[i].m_x = x[i] * 10 - 90;		//�����ĺ�����
			bullet[i].m_y = (y[i] + 60) * 10;	//������������
			bullet[i].m_hx = bullet[i].m_x;		//��ߵ������
			bullet[i].m_hy = y[i] * 10;			//��ߵ�������
			bullet[i].height = bullet[i].m_y - bullet[i].m_hy;//�߶�һ��

			bullet[i].shoot = 1;
			putimage(bullet[i].m_x, bullet[i].m_y, &bullet[i].imgs[bullet[i].flag], SRCINVERT);
		}
	}
}

void MouseMsg()
{
	static int index = 0;
	msg = GetMouseMsg();  //��ȡ�����Ϣ
	switch (msg.uMsg)    //�������Ϣ����ƥ��
	{
	case WM_LBUTTONDOWN:  //�������Ϣ���������ʱ
	{
		//int i = rand() % NUM;	//13���̻����������ȡһ��
		if (bullet[index].shoot == 0 && flower[index].draw == 0)
		{
			bullet[index].m_x = msg.x;
			bullet[index].m_y = rand() % 100 + 600; //600~699
			bullet[index].m_hx = bullet[index].m_x;
			bullet[index].m_hy = rand() % 300;
			bullet[index].shoot = 1;	//����״̬
			putimage(bullet[index].m_x, bullet[index].m_y, &bullet[index].imgs[bullet[index].flag], SRCINVERT);
			index = index++ % 13;
		}

	}
		break;
	default:
		break;
	}
}
