#if 0
#include<easyx.h>
#include<stdio.h>
#include<time.h>

#define WIDTH 10
#define HEIGHT 22
#define PIX 20

int g_map[HEIGHT][WIDTH] = { 0 };

enum OPERATE
{
	ROTATE	//��ת	����
	, LEFT	//����	���
	, RIGHT	//����	�Ҽ�
	, DOWN	//����	����
	, SINK	//�³�	�ո��
	, QUIT	//�˳�	Esc��
	, STOP	//��ͣ	o��
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
	WORD dir[4];
	COLORREF color;
}g_Blocks[7] = {
	{ 0x0F00, 0x4444, 0x0F00, 0x4444, RED },	//I
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
	byte id;		//�ڼ�������
	byte dir : 2;	//�Ƿ����еĵڼ���00 01 10 11
	char x, y;		//��ǰ���ӵ�λ��������
}g_CurBlock, g_NextBlock;

void init();
void NewGame();
void NewBlock();
void DrawBlock(BLOCKINFO block);
void DrawUnit(int x, int y, COLORREF color);

int main()
{
	initgraph(640, 480);
	init();
	getchar();
	closegraph();
	return 0;
}

void init()
{
	srand((unsigned)time(NULL));	//�����������
	setbkmode(TRANSPARENT);			//���ñ�����ɫΪ͸��

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
	//����������
	setorigin(220, 20);
	rectangle(0, 0, WIDTH*PIX, HEIGHT*PIX);
	rectangle((WIDTH + 1)*PIX, 0, (WIDTH + 6)*PIX, 6 * PIX);

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
	//NewBlock();
}


void NewBlock()
{
	g_CurBlock.dir = g_NextBlock.dir;
	g_NextBlock.dir = 3;

	g_CurBlock.id = g_NextBlock.id;
	g_NextBlock.id = 6;

	g_CurBlock.x = WIDTH / 2;
	g_CurBlock.y = HEIGHT + 2;

	//��ȡg_CurBlock�ķ�����Ϣ
	WORD c = g_Blocks[g_CurBlock.id].dir[g_CurBlock.dir];

	while ((c & 0x000F) == 0)
	{
		g_CurBlock.y--;
		c >>= 4;
	}
	DrawBlock(g_NextBlock);
	g_NextBlock.id = 6;	//����ͼ�������һ��
	g_NextBlock.dir = 2;	//�ĸ����������һ��
	g_NextBlock.x = WIDTH + 1 -5;
	g_NextBlock.y = HEIGHT - 2;
	DrawBlock(g_NextBlock);
	g_NextBlock.id = 6;	//����ͼ�������һ��
	g_NextBlock.dir = 1;	//�ĸ����������һ��
	g_NextBlock.x = WIDTH + 1 - 5 - 5;
	g_NextBlock.y = HEIGHT - 2;
	DrawBlock(g_NextBlock);
	g_NextBlock.id = 6;	//����ͼ�������һ��
	g_NextBlock.dir = 0;	//�ĸ����������һ��
	g_NextBlock.x = WIDTH + 1 - 5 - 5 - 5;
	g_NextBlock.y = HEIGHT - 2;
	DrawBlock(g_NextBlock);
	//DrawBlock(g_CurBlock);
}

void DrawBlock(BLOCKINFO block)
{
	WORD b = g_Blocks[block.id].dir[block.dir];
	int x, y;

	//��16�������е�ÿһ��
	for (int i = 0; i < 16; i++, b <<= 1)
	{
		if (b & 0x8000)
		{
			x = block.x + i % 4;
			y = block.y - i / 4;
			if (y < HEIGHT)
				DrawUnit(x, y, g_Blocks[block.id].color);
		}
		else
		{
			x = block.x + i % 4;
			y = block.y - i / 4;
			DrawUnit(x, y, WHITE);
		}
	}
}

void DrawUnit(int x, int y, COLORREF color)
{
	int left = x * PIX;
	int right = (x + 1) * PIX - 1;
	int top = (HEIGHT - y - 1) * PIX;
	int bottom = (HEIGHT - y) * PIX;

	setfillcolor(color);
	setlinecolor(LIGHTGRAY);
	fillrectangle(left, top, right, bottom);
}

#endif

#include<easyx.h>
#include<stdio.h>
#include<time.h>

#define WIDTH 10
#define HEIGHT 22
#define PIX 20

int g_map[HEIGHT][WIDTH] = { 0 };

enum OPERATE
{
	ROTATE	//��ת	����
	, LEFT	//����	���
	, RIGHT	//����	�Ҽ�
	, DOWN	//����	����
	, SINK	//�³�	�ո��
	, QUIT	//�˳�	Esc��
	, STOP	//��ͣ	o��
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
	WORD dir[4];
	COLORREF color;
}g_Blocks[7] = {
	{ 0x0F00, 0x4444, 0x0F00, 0x4444, RED },	//I
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
	byte id;		//�ڼ�������
	byte dir : 2;	//�Ƿ����еĵڼ���00 01 10 11
	char x, y;		//��ǰ���ӵ�λ��������
}g_CurBlock, g_NextBlock;

void init();
void NewGame();
void NewBlock();
void DrawBlock(BLOCKINFO block);
void DrawUnit(int x, int y, COLORREF color);

int main()
{
	initgraph(640, 480);
	init();
	getchar();
	closegraph();
	return 0;
}

void init()
{
	srand((unsigned)time(NULL));	//�����������
	setbkmode(TRANSPARENT);			//���ñ�����ɫΪ͸��

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
	//����������
	setorigin(220, 20);
	rectangle(0, 0, WIDTH*PIX, HEIGHT*PIX);
	rectangle((WIDTH + 1)*PIX, 0, (WIDTH + 6)*PIX, 6 * PIX);

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

	g_CurBlock.x = WIDTH / 2;
	g_CurBlock.y = HEIGHT + 2;

	//��ȡg_CurBlock�ķ�����Ϣ
	WORD c = g_Blocks[g_CurBlock.id].dir[g_CurBlock.dir];

	while ((c & 0x000F) == 0)
	{
		g_CurBlock.y--;
		c >>= 4;
	}
	DrawBlock(g_NextBlock);
	DrawBlock(g_CurBlock);
}

void DrawBlock(BLOCKINFO block)
{
	WORD b = g_Blocks[block.id].dir[block.dir];
	int x, y;

	//��16�������е�ÿһ��
	for (int i = 0; i < 16; i++, b <<= 1)
	{
		x = block.x + i % 4;
		y = block.y - i / 4;
			DrawUnit(x, y, WHITE);
		if (b & 0x8000)
		{
			x = block.x + i % 4;
			y = block.y - i / 4;
			if (y < HEIGHT)
				DrawUnit(x, y, g_Blocks[block.id].color);
		}
	}
}

void DrawUnit(int x, int y, COLORREF color)
{
	int left = x * PIX;
	int right = (x + 1) * PIX - 1;
	int top = (HEIGHT - y - 1) * PIX;
	int bottom = (HEIGHT - y) * PIX;

	setfillcolor(color);
	setlinecolor(LIGHTGRAY);
	fillrectangle(left, top, right, bottom);
}
