#include<easyx.h>
#include<stdio.h>
#include<time.h>

#define WIDTH 10
#define HEIGHT 22
#define PIX 20

int g_map[HEIGHT][WIDTH] = { 0 };

enum OPERATE
{
	ROTATE	//旋转	↑键
	,LEFT	//左移	左键
	,RIGHT	//右移	右键
	,DOWN	//下移	↓键
	,SINK	//下沉	空格键
	,QUIT	//退出	Esc键
	,STOP	//暂停	o键
};

//图像下落中的状态
enum FLAG {
	SHOW,//正常显示
	CLEAR,//擦除
	FIX//固定
};

//方块结构体
struct BLOCK
{
	WORD dir[4];
	COLORREF color;
}g_Blocks[7] = {
	{ 0x0F00, 0x4444, 0x0F00, 0x4444, RED },	//I
	{ 0x0660, 0x0660, 0x0660, 0x0660, BLUE },	//口
	{ 0x4460,0x02E0,0x0622,0x0740,BLUE },		//L
	{ 0x2260,0x0E20,0x0644,0x0470,RED },		//反L
	{ 0x0c60,0x2640,0x0c60,0x2640,YELLOW },		//Z
	{ 0x0360, 0x4620, 0x0360, 0x4620, YELLOW },	//反z
	{ 0x4E00, 0X4C40, 0x0E40,0X4640, BLUE }		//T
};

//方块信息
struct BLOCKINFO
{
	byte id;		//第几个方块
	byte dir : 2;	//是方块中的第几个00 01 10 11
	char x, y;		//当前格子的位置在哪里
}g_CurBlock, g_NextBlock;

void init();
void NewGame();
void NewBlock();
void DrawBlock(BLOCKINFO block);
void DrawUnit(int x,int y,COLORREF color);

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
	srand((unsigned)time(NULL));	//设置随机种子
	setbkmode(TRANSPARENT);			//设置背景颜色为透明

	//显示文字
	settextstyle(14, 0, L"宋体");
	outtextxy(20, 280, L"操作说明");
	outtextxy(20, 300, L"上：  旋转");
	outtextxy(20, 320, L"下：  下落");
	outtextxy(20, 340, L"左：  左移");
	outtextxy(20, 360, L"右：  右移");
	outtextxy(20, 380, L"空格：快速下落");
	outtextxy(20, 400, L"p：   暂停");
	outtextxy(20, 420, L"r:    重新开始");
	outtextxy(20, 440, L"esc： 退出程序");
	//画两个矩形
	setorigin(220, 20);
	rectangle(0, 0, WIDTH*PIX, HEIGHT*PIX);
	rectangle((WIDTH + 1)*PIX, 0, (WIDTH + 6)*PIX, 6 * PIX);

	NewGame();
}

void NewGame()
{
	//随机产生图形
	setfillcolor(BLACK);
	ZeroMemory(g_map, WIDTH*HEIGHT * sizeof(int));
	g_NextBlock.id = rand() % 7;	//七种图形中随机一个
	g_NextBlock.dir = rand() % 4;	//四个方向中随机一个
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
	g_CurBlock.y =  HEIGHT + 2;

	//获取g_CurBlock的方块信息
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

	//画16个格子中的每一个
	for (int i = 0; i < 16; i++, b <<= 1)
	{
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
