#if 1
/******************************************************************************

��Ȩ���� (C), 2001-2011, DeRoy

******************************************************************************
�� �� ��   : ����˹����������.c
�� �� ��   : ����
��    ��   : DeRoy
��������   : 2020��2��22��
����޸�   :
��������   : ����˹����Easyxͼ�ο�ʵ��
�����б�   :
*
*       1.                CheckBlock
*       2.                DrawBlock
*       3.                DrawUnit
*       4.                init
*       5.                main
*       6.                NewBlock
*       7.                NewGame
*       8.                OnDown
*       9.                OnLeft
*       10.                OnRight
*       11.                OnRotate
*       12.                OnSink
*       13.                Restart
*       14.                Rotate
*       15.                Select
*       16.                Stop
*

�޸���ʷ   :
1.��    ��   : 2020��2��22��
��    ��   : DeRoy
�޸�����   : �����ļ�

******************************************************************************/
#include<easyx.h>
#include<stdio.h>
#include<time.h>
#include<conio.h>

#define WIDTH 10
#define HEIGHT 22
#define PIX 20

int g_map[HEIGHT][WIDTH] = { 0 };
int g_score;

enum OPERATE
{
	ROTATE	//��ת	����
	,LEFT	//����	���
	,RIGHT	//����	�Ҽ�
	,DOWN	//����	����
	,SINK	//�³�	�ո��
	,QUIT	//�˳�	Esc��
	,STOP	//��ͣ	o��
};

//ͼ�������е�״̬
enum FLAG {
	SHOW,//������ʾ
	CLEAR,//����
	FIX//�̶�
};

//����ṹ��
struct BLOCK
{
	int dir[4];
	COLORREF color;
}g_Blocks[7] = {
	{ 0x0f00, 0x4444, 0x0f00, 0x4444, RED },	//I
	{ 0x0660, 0x0660, 0x0660, 0x0660, BLUE },	//��
	{ 0x4460,0x02E0,0x0622,0x0740,BLUE },		//L
	{ 0x2260,0x0E20,0x0644,0x0470,RED },		//��L
	{ 0x0c60,0x2640,0x0c60,0x2640,YELLOW },		//Z
	{ 0x0360, 0x4620, 0x0360, 0x4620, YELLOW },	//��z
	{ 0x4E00, 0X4C40, 0x0E40,0X4640, BLUE }		//T
};

//������Ϣ
struct BLOCKINFO
{
	int id;		//�ڼ�������
	int dir;	//�Ƿ����еĵڼ������� 0 1 2 3
	char x, y;		//��ǰ���ӵ�λ��������
}g_CurBlock, g_NextBlock;

void init();
void NewGame();
void NewBlock();
void DrawBlock(BLOCKINFO block, FLAG flag = SHOW);
void DrawUnit(int x,int y,COLORREF color, FLAG flag);
void Select();
void OnRight();
void OnLeft();
void OnDown();
bool CheckBlock(BLOCKINFO block);
void OnSink();
void OnRotate();
void Rotate(int dx);
void Stop();
void Restart();

int main()
{
	initgraph(640, 480);
	init();
	while (1)
	{
		Select();
	}
	getchar();
	closegraph();
	return 0;
}

void init()
{
	srand((unsigned)time(NULL));	//�����������
	setbkmode(TRANSPARENT);			//���ñ�����ɫΪ͸��
	setorigin(0, 0);				//
	cleardevice();					//����
	//��ʾ����
	settextstyle(14, 0, L"����");
	outtextxy(20, 280, L"����˵��");
	outtextxy(20, 300, L"�ϣ�  ��ת");
	outtextxy(20, 320, L"�£�  ����");
	outtextxy(20, 340, L"��  ����");
	outtextxy(20, 360, L"�ң�  ����");
	outtextxy(20, 380, L"�ո񣺿�������");
	outtextxy(20, 400, L"p��   ��ͣ");
	outtextxy(20, 420, L"r:    ���¿�ʼ");
	outtextxy(20, 440, L"esc�� �˳�����");

	outtextxy(460, 280, L"���ڳ���");
	outtextxy(460, 320, L"VX���ں�:���ѧϰ����");
	outtextxy(460, 360, L"QQ����Ⱥ:879098023");
	outtextxy(460, 440, L"���ս���Ȩ���η�����");

	g_score = 0;
	//����������
	setorigin(220, 20);
	setlinecolor(WHITE);
	rectangle(-1, -1, WIDTH*PIX, HEIGHT*PIX);
	rectangle((WIDTH + 1)*PIX - 1, 0, (WIDTH + 6)*PIX, 6 * PIX);

	NewGame();
}

void NewGame()
{
	//�������ͼ��
	setfillcolor(BLACK);
	ZeroMemory(g_map, WIDTH*HEIGHT * sizeof(int));
	g_NextBlock.id = rand() % 7;	//����ͼ�������һ��
	g_NextBlock.dir = rand() % 4;	//�ĸ����������һ��
	g_NextBlock.x = WIDTH + 1;
	g_NextBlock.y = HEIGHT - 2;
	NewBlock();
}

void NewBlock()
{
	g_CurBlock.dir = g_NextBlock.dir;
	g_NextBlock.dir = rand() % 4;

	g_CurBlock.id = g_NextBlock.id;
	g_NextBlock.id = rand() % 7;

	g_CurBlock.x = WIDTH / 2 - 1;
	g_CurBlock.y = HEIGHT + 2;

	//��ȡg_CurBlock�ķ�����Ϣ
	int c = g_Blocks[g_CurBlock.id].dir[g_CurBlock.dir];
	//0x4444&0x000F
	//0x0004 ! = 0	˵��ʮ���������һ�в�ΪΪ�գ���ʾg_CurBlock��ͼ�ο�������Ļ����ʾ���һ��
	while ((c & 0x000F) == 0)
	{
		g_CurBlock.y--;
		c >>= 4;
	}

	setfillcolor(BLACK);
	solidrectangle((WIDTH + 1)*PIX, 1, (WIDTH + 6)*PIX - 1, 6 * PIX - 1);
	DrawBlock(g_NextBlock, CLEAR);
	DrawBlock(g_NextBlock);
	DrawBlock(g_CurBlock);
}

void DrawBlock(BLOCKINFO block, FLAG flag)
{
	int b = g_Blocks[block.id].dir[block.dir];
	int x, y;

	//��16�������е�ÿһ��
	for (int i = 0; i < 16; i++, b <<= 1)
	{
		//1000 0000 0000 0000
		//0100 0100 0100 0100	->0x4444 I�ͷ���  ->b����һλ 1000 1000 1000 1000 
		//1000 0000 0000 0000 & 1000 1000 1000 1000 ->Ϊ����Ƶڶ������� ->ѭ���������
		if (b & 0x8000)
		{
			x = block.x + i % 4;	
			y = block.y - i / 4;
			if (y < HEIGHT)
				DrawUnit(x, y, g_Blocks[block.id].color,flag);
		}
	}
}

void DrawUnit(int x, int y, COLORREF color, FLAG flag)
{
	int left = x * PIX ;			// + 1 ��Ϊ�˷�ֹ�����߿���
	int right = (x + 1) * PIX - 1;	// -1 ��Ϊ�˷�ֹ�����߿���
	int top = (HEIGHT - y - 1) * PIX;
	int bottom = (HEIGHT - y) * PIX - 1;
	switch (flag)
	{
	case SHOW:
		setfillcolor(color);
		setlinecolor(LIGHTGRAY);
		fillrectangle(left, top, right, bottom);
		break;
	case CLEAR://����
		setfillcolor(BLACK);
		solidrectangle(left, top, right, bottom);
		break;
	case FIX: //�̶�
		setfillcolor(RGB(GetRValue(color) * 2 / 3, GetGValue(color) * 2 / 3, GetBValue(color) * 2 / 3));
		setlinecolor(DARKGRAY);
		fillrectangle(left, top, right, bottom);
		break;

	default:
		break;
	}
}

DWORD oldTime;
void Select()
{
	DWORD newTime = GetTickCount();
	
	if (newTime - oldTime > 500)
	{
		oldTime = newTime;
		OnDown();
	}
	if (kbhit())
	{
		switch (getch())
		{
		case 'w':
		case 'W':	
		case 72:	
			OnRotate();//��ת
			break;
		case 'a':
		case 'A':	
		case 75:	
			OnLeft();	//����
			break;
		case 'd':
		case 'D':	
		case 77:	
			OnRight();	//����
			break;
		case 's':
		case 'S':	
		case 80:	
			OnDown();	//�½�
			break;
		case 27:		//Esc�� 
			if (MessageBox(GetHWnd(), L"�Ƿ��˳���Ϸ��", L"����", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
			{
				closegraph();
				exit(0);	//�˳���Ϸ
			}
			break;
		case ' ':	
 			OnSink();		//�³�
 			break;
		case 'p':
			Stop();			//��ͣ
				break;
		case 'r':
			Restart();		//���¿�ʼ
			break;
		}
	}
	wchar_t ScoreArr[10];
	wsprintf(ScoreArr, L"������%d", g_score);
	settextcolor(WHITE);	
	setbkmode(OPAQUE);	
	settextstyle(18, 0, L"����");	
	outtextxy(280, 160, ScoreArr);	
	Sleep(20);
}

void OnRight()
{
	BLOCKINFO temp = g_CurBlock;
	temp.x++;
	if (CheckBlock(temp))	//�Ƿ�����ƶ�
	{
		DrawBlock(g_CurBlock, CLEAR);
		g_CurBlock.x++;
		DrawBlock(g_CurBlock);
	}
}

void OnLeft()
{
	BLOCKINFO temp = g_CurBlock;
	temp.x--;
	if (CheckBlock(temp))//�Ƿ����
	{
		DrawBlock(g_CurBlock, CLEAR);
		g_CurBlock.x--;
		DrawBlock(g_CurBlock);
	}
}

void OnDown()
{
	BLOCKINFO temp = g_CurBlock;
	temp.y--;
	if (CheckBlock(temp))
	{
		DrawBlock(g_CurBlock, CLEAR);
		g_CurBlock.y--;
		DrawBlock(g_CurBlock);
	}
	else
		OnSink();	// ��������ʱ��ִ�С����׷��顱����
}

bool CheckBlock(BLOCKINFO block)
{
	int b = g_Blocks[block.id].dir[block.dir];
	int x, y;

	for (int i = 0; i < 16; i++, b <<= 1)
		if (b & 0x8000)
		{
			x = block.x + i % 4;
			y = block.y - i / 4;
			if ((x < 0) || (x >= WIDTH) || (y < 0))
				return false;

			if ((y < HEIGHT) && (g_map[y][x]))
				return false;
		}
	return true;
}

void OnSink()
{
	int i, x, y;

	//�������Ʒ���
	DrawBlock(g_CurBlock, CLEAR);
	BLOCKINFO temp = g_CurBlock;
	temp.y--;
	while (CheckBlock(temp))
	{
		g_CurBlock.y--;
		temp.y--;
	}
	DrawBlock(g_CurBlock, FIX);

	// �̶���������Ϸ��
 	int b = g_Blocks[g_CurBlock.id].dir[g_CurBlock.dir];
	for (i = 0; i < 16; i++, b <<= 1)
		if (b & 0x8000)
		{
			if (g_CurBlock.y - i / 4 >= HEIGHT)
			{	//�������Ĺ̶�λ�ó����߶ȣ�������Ϸ
				if (MessageBox(GetHWnd(), L"�Ƿ����¿�ʼ��", L"����", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
				{
					init();
					return; 
				}
				else
				{
					closegraph();
					exit(0);
				}
			}
			else
				g_map[g_CurBlock.y - i / 4][g_CurBlock.x + i % 4] = 1;
		}

	// ����Ƿ���Ҫ�����У������
	int remove = 0;	// �� 4 λ������Ƿ����漰�� 4 ���Ƿ���������Ϊ
	for (y = g_CurBlock.y; y >= g_CurBlock.y - 3, y>=0; y--)
	{
		i = 0;	//ͳ��һ�и�������
		for (x = 0; x < WIDTH; x++)
			if (g_map[y][x] == 1)
				i++;

		if (i == WIDTH)
		{
			remove |= (1 << (g_CurBlock.y - y));
   			setfillcolor(GREEN);	//��ɫ
			setlinecolor(GREEN);
			fillrectangle(0, (HEIGHT - y - 1) * PIX , WIDTH * PIX - 1, (HEIGHT - y) * PIX - 1 );
			g_score += 10;	//����һ�гɼ���10
		}
	}

	if (remove)	// ���������������
	{
		// ��ʱ 300 ����
		Sleep(300);

		//�����ղű�ǵ���
		IMAGE img;
		for (i = 0; i < 4; i++, remove >>= 1)
		{
			if (remove & 1)
			{
				for (y = g_CurBlock.y - i + 1; y < HEIGHT; y++)
					for (x = 0; x < WIDTH; x++)
					{
						g_map[y - 1][x] = g_map[y][x];
						g_map[y][x] = 0;
					}
				getimage(&img, 0, 0, WIDTH * PIX, (HEIGHT - (g_CurBlock.y - i + 1)) * PIX);
				putimage(0, PIX, &img);
			}
		}
	}

	//�����·���
	NewBlock();
}

void OnRotate()
{
	//��ȡ������ת�� x ƫ����
	int dx;
	BLOCKINFO tmp = g_CurBlock;
	tmp.dir = (++tmp.dir) % 4;	//���Ʒ�����0��3֮��
	if (CheckBlock(tmp)) 
	{ 
		dx = 0;		Rotate(dx); return;
	}

	tmp.x = g_CurBlock.x - 1;	if (CheckBlock(tmp)) { 
		dx = -1;	Rotate(dx); return;
	}

	tmp.x = g_CurBlock.x + 1;	if (CheckBlock(tmp)) { 
		dx = 1;		Rotate(dx); return;
	}

	tmp.x = g_CurBlock.x - 2;	if (CheckBlock(tmp)) { 
		dx = -2;	Rotate(dx); return;
	}

	tmp.x = g_CurBlock.x + 2;	if (CheckBlock(tmp)) { 
		dx = 2;		Rotate(dx); return;
	}
}

void Rotate(int dx)
{
	// ��ת
	DrawBlock(g_CurBlock, CLEAR);
	g_CurBlock.dir = (++g_CurBlock.dir) % 4;	//���Ʒ�����0��3֮��
	g_CurBlock.x += dx;
	DrawBlock(g_CurBlock);
}

void Stop()
{
	char ch;
	do
	{
		ch = getch();
	} while (ch != 'p');
}

void Restart()
{
	if (MessageBox(GetHWnd(), L"�Ƿ����¿�ʼ��", L"����", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		init();
	}
}
#endif