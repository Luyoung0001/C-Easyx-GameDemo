#include<stdio.h>
#include<easyx.h>	//图形库
#include<time.h>	//随机数

#define N 4

IMAGE img[4],imgs[16];		//img[4] 4张原图，四个关卡 imgs分割图片
int aim_c, aim_r;		//空白处目标所在位置	行 列
int map[4][4] = { 0 };	//地图初始化为0
int NUM = 0;			//第一张图片储存在img[0]处

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
	//加载资源图片
	loadimage(&img[0], L"images/1.jpg", 400, 400);
	loadimage(&img[1], L"images/2.jpg", 400, 400);
	loadimage(&img[2], L"images/3.jpg", 400, 400);
	loadimage(&img[3], L"images/4.jpg", 400, 400);
	// 设置绘图目标为 img 对象
	SetWorkingImage(&img[NUM]);
	for (int y = 0, n = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			getimage(&imgs[n++], x * 100, y * 100, (x + 1) * 100, (y + 1) * 100);
		}
	}
	// 设置绘图目标为绘图窗口
	SetWorkingImage();
	//设置最后一张图片为空白图片，作为目标图片
	loadimage(&imgs[15], L"images/15.jpg", 400, 400);
	putimage(0,0,&img[NUM]);

	//设置随机种子
	srand((unsigned)time(NULL));
	//初始化地图 0~15
	for (int i = 0, k = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			map[i][j] = k++;
		}
	}
	//打乱地图
	for (int i = 0; i <= 100; i++)
	{
		//得到目标所在行和列
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (map[i][j] == 0)	//第一张裁剪图片作为交换目标
				{
					aim_r = i;
					aim_c = j;
					break;
				}
			}
		}
		int dir = rand() % 4;		//随机一个方向
		switch (dir)
		{
		case 0:	//向上交换
			if (aim_r >= 1)
			{
				map[aim_r][aim_c] = map[aim_r - 1][aim_c];
				map[aim_r - 1][aim_c] = 0;
			}
			break;
		case 1:	//向下交换
			if (map[aim_r + 1][aim_c] == 15) break;	//不和空白图片交换
			if (aim_r < 3)
			{
				map[aim_r][aim_c] = map[aim_r + 1][aim_c];
				map[aim_r + 1][aim_c] = 0;
			}
			break;
		case 2:	//向左交换
			if (aim_c >= 1)
			{
				map[aim_r][aim_c] = map[aim_r][aim_c - 1];
				map[aim_r][aim_c - 1] = 0;
			}
			break;
		case 3:	//向右交换
			if (map[aim_r][aim_c + 1] == 15) break;	//不和空白图片交换
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
	int col, row;			//鼠标点击的位置 行 列
	MOUSEMSG msg;			//鼠标消息
	msg = GetMouseMsg();	//获取鼠标消息
	switch (msg.uMsg)		//对鼠标消息进行匹配
	{
	case WM_LBUTTONDOWN:	//当鼠标消息是左键按下时
		//获取鼠标按下所在列
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

		//获取鼠标按下所在行
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
		//得到目标所在行和列
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (map[i][j] == 15)	//空白处为交换目标
				{
					aim_r = i;
					aim_c = j;
				}
			}
		}
		//判难鼠标点击位置和目标是否相邻，相邻交换数据
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
			MessageBox(NULL, L"挑战成功", L"Vectory", MB_OK);
		}
		if (MessageBox(NULL, L"是否进入下一关", L"Vectory", MB_YESNO) == IDYES)
		{
			GameInit();	//游戏初始化
		}
		else  exit(0);
	}	
}