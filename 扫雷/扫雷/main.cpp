#include<stdio.h>
#include<easyx.h>
#include<time.h>
/**************全局变量**************/
int map_row, map_col;	//地图大小
int times;	//翻开的此时
int mine;	//雷的数量
int isfail;	//判断是否失败
int pass;	//雷的数量达到10% 就扩大游戏区域
HWND hWnd;	//当前窗口
int map[30][30] = { 0 };	//地图	用map_row map_col控制地图大小
IMAGE imgs[14];	//图片
/**************全局函数**************/
void init();
void DrawMap();
void initmap();	//赢了之后初始化游戏数据 更新地图
void Play();
void Judg();
void Empty(int row,int col);

int main()
{
	init();		//初始化
	DrawMap();	//绘制图形
	while (1)
	{
		Play();	//玩家操作
		Judg();	//判断输赢
	}
	getchar();
	closegraph();
	return 0;
}

void init()
{
	srand((unsigned)time(NULL));	//设置随机种子
	map_row = 10;	//地图行
	map_col = 10;	//地图列
	isfail = 0;
	pass = 0;		
	//加载图片
	loadimage(&imgs[0], L"素材/0.bmp", 25, 25);
	loadimage(&imgs[1], L"素材/1.bmp", 25, 25);
	loadimage(&imgs[2], L"素材/2.bmp", 25, 25);
	loadimage(&imgs[3], L"素材/3.bmp", 25, 25);
	loadimage(&imgs[4], L"素材/4.bmp", 25, 25);
	loadimage(&imgs[5], L"素材/5.bmp", 25, 25);
	loadimage(&imgs[6], L"素材/6.bmp", 25, 25);
	loadimage(&imgs[7], L"素材/7.bmp", 25, 25);
	loadimage(&imgs[8], L"素材/8.bmp", 25, 25);
	loadimage(&imgs[9], L"素材/标记.bmp", 25, 25);
	loadimage(&imgs[10], L"素材/空.bmp", 25, 25);
	loadimage(&imgs[11], L"素材/雷0.bmp", 25, 25);
	loadimage(&imgs[12], L"素材/雷1.bmp", 25, 25);
	loadimage(&imgs[13], L"素材/雷2.bmp", 25, 25);

	initmap();
}

void DrawMap()
{
	BeginBatchDraw();
	for (int y = 1; y <= map_row; y++)
	{
		for (int x = 1; x <= map_col; x++)
		{
			if (map[y][x] > 18)
			{
				//标记部分额外处理
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[9]);
				continue;
			}
			switch (map[y][x])
			{
			//制作初期可以加上下面代码看看雷放到了哪里
			//case -1:	//雷
			//	putimage(x * 25, y * 25, &imgs[12]);
			//	break;
			case 9:		// -1 + 10 = 9 踩到雷了
				putimage((x - 1) *25, (y - 1) *25, &imgs[13]);
				isfail = 1;
				break;
			case 10:	//没有雷
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[0]);
				break;
			case 11:	//有一个雷
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[1]);
				break;
			case 12:	//有两个雷
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[2]);
				break;
			case 13:	//有三个雷
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[3]);
				break;
			case 14:	//有四个雷
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[4]);
				break;
			case 15:	//有五个雷
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[5]);
				break;
			case 16:	//有六个雷
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[6]);
				break;
			case 17:	//有七个雷
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[7]);
				break;
			case 18:	//有八个雷
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[8]);
				break;
			default:
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[10]);
				break;
			}
		}
	}
	EndBatchDraw();
}

void initmap()
{
	ZeroMemory(map, sizeof(int) * 30 * 30);		//初始化地图为0
	times = 0;	//翻开次数
	mine += 5;	//雷的数量 + 5
	float x = mine / (float)(map_row * map_col);
	if ( x > 0.15)
	{
		//当雷的数量达到10%时扩大游戏区域	先横向扩大5个后纵向扩大5个
		if (pass++ % 2)		//使 pass 在 0 和 1之间变换		首先是0 ，然后0 1 之间循环
		{
			map_row += 5;	//纵向扩大5个区域
		}
		else
		{
			map_col += 5;	//横向扩大5个区域
		}
	}

	hWnd = initgraph(map_col * 25, map_row * 25);		//初始化地图大小

	int mine_r, mine_c;		//雷所在行和列
	for (int i = 0; i < mine;)
	{
		mine_r = rand() % map_row + 1;	//mine_r 1 ~ 8
		mine_c = rand() % map_col + 1;
		if (map[mine_r][mine_c] == 0)
		{
			map[mine_r][mine_c] = -1;	//在数组中放雷
			i++;						//放入雷后雷的数量加一
		}
	}
	//给雷附近的数加一
	for (int i = 1; i <= map_row; i++)
	{
		for (int j = 1; j <= map_col; j++)
		{
			if (map[i][j] != -1)
			{
				//检查周围雷的个数
				for (int m = i - 1; m <= i + 1; m++)
				{
					for (int n = j - 1; n <= j + 1; n++)
					{
						if (map[m][n] == -1) {	//附近有雷，数字加一
							map[i][j]++;
						}
					}
				}
			}
		}
	}

	wchar_t arr[20];
	wsprintf(arr, L"扫雷 雷:%d", mine);
	SetWindowText(hWnd, arr);
}

void Play()
{
	MOUSEMSG msg;//鼠标消息
	int row, col;//鼠标点击的行和列
	msg = GetMouseMsg();	//获取鼠标消息
	row = msg.y / 25 + 1;	//鼠标点击行	
	col = msg.x / 25 + 1;	//鼠标点击列
	switch (msg.uMsg)
	{
	case WM_LBUTTONDOWN://左键按下
		if (map[row][col] >= 9)//翻开过的 和标记的 不进行操作
		{
			break;
		}
		if (map[row][col] == 0)//翻开 0项 和 周围的图片
		{
			Empty(row,col);
		}
		else//翻开点击的图片
		{
			map[row][col] += 10;//翻开
			times++;	//翻开次数 +1
		}
		break;
	case WM_RBUTTONDOWN://右键按下 WM_RBUTTONDOWN
		if (map[row][col] >= 9 && map[row][col]<19)//翻开的不进行操作
		{
			break;
		}
		if (map[row][col] >= 19)//标记的取消标记
		{
			map[row][col] -= 20;
		}
		else//没有翻开的加20
		{
			map[row][col] += 20;	//20是随意的数字，只要足够大就行
		}
		break;
	}
	DrawMap();
}

void Judg()
{
	//先查找是否点开了雷
	if (isfail)
	{
		if (MessageBox(GetHWnd(), L"你踩到雷了,是否重新开始本关", L"失败", MB_YESNO) == IDYES)		//GetHWnd()是获取一个顶置窗口
		{
			mine -= 5;
			isfail = 0;
			initmap();
			DrawMap();	//渲染地图
		}
		else
		{
			_exit(0);
		}
	}

	if (times == map_row * map_col - mine)	//所有点 - 雷 = 非雷区
	{
		if (MessageBox(GetHWnd(), L"是否进入下一关", L"成功", MB_YESNO) == IDYES) {
			closegraph();
			//重新开始游戏
			initmap();
			DrawMap();	//渲染地图
		}
		else
			_exit(0);
	}
}

void Empty(int row, int col)
{
	//递归实现以 0 为 九宫格的区域 自动翻开
	map[row][col] += 10;	//翻开当前位置
	times++;				//翻开次数+1
	for (int i = row - 1; i <= row + 1; i++)
	{
		for (int j = col - 1; j <= col + 1; j++)
		{
			//外圈空白区域不操作	仅对可以看到的区域进行递归调用
			if (i >= 1 && i <= map_row && j >= 1 && j <= map_col)
			{
				if (map[i][j] < 9)			//小于9 是未操作，未翻开的区域
				{
					if (map[i][j] == 0)		//0的周围九宫格内无雷，翻开
					{
						Empty(i, j);
					}
					else
					{
						map[i][j] += 10;	//有雷的不翻，让玩家思考是否需要翻开
						times++;
					}
				}
			}
		}
	}
}


