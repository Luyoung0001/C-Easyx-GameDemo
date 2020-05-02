#include<stdio.h>
#include<easyx.h>
#include<conio.h>
#include<Windows.h>
#pragma  comment (lib,"Winmm.lib") //使用音乐播放库

#define MAP_R 11	//地图行数
#define MAP_C 13	//地图列数

/**********加载全局函数********/
void InitGame();		//初始化游戏  加载图片
void RenderGame();		//绘制游戏
void UpdateGame();		//用户输入按键  进行数据更新
bool isWin();			//判断是否赢
bool isFail();			//判断是否输
/**********加载全局变量********/
IMAGE img[7];				//六张图片
char dir;					//按键    上下左右四个按键
int index_Row, index_Col;	//人所在行，列
DWORD T1, T2;				//T1:按键时间,T2下次按键时间
int flag;					//人的状态	制作动态人物
int map[MAP_R][MAP_C] = {			//初始化地图
	0,0,0,1,1,1,1,1,1,1,0,0,0,
	1,1,1,1,0,0,0,0,0,1,0,0,0,
	1,0,0,0,4,1,1,1,0,1,0,0,0,
	1,0,1,0,1,0,0,0,0,1,1,0,0,
	1,0,1,0,2,0,2,1,4,0,1,0,0,
	1,0,1,0,0,6,0,0,1,0,1,0,0,
	1,0,4,1,2,0,2,0,1,0,1,0,0,
	1,1,0,0,0,0,1,0,1,0,1,1,1,
	0,1,0,1,1,1,4,0,0,0,0,0,1,
	0,1,0,0,0,0,0,1,1,0,0,3,1,
	0,1,1,1,1,1,1,1,1,1,1,1,1
};


int main()
{
	initgraph(40 * MAP_C, 40 * MAP_R);
	InitGame();				//加载游戏
	RenderGame();			//绘制游戏
	T1 = GetTickCount();
	while (1)
	{
		T2 = GetTickCount();
		if (T2 - T1 >= 300)		//0.3s绘制一次游戏
		{
			T1 = T2;			//交换时间，进行下一次循环
			flag = ++flag % 2;
			RenderGame();		//绘制游戏
		}
		UpdateGame();		//数据更新
		if (isWin())
		{
			MessageBox(GetHWnd(), L"恭喜你赢了", L"Vectory", MB_OK);
			break;
		}
		if (isFail())
		{
			MessageBox(GetHWnd(), L"抱歉你输了", L"Defeated", MB_OK);
			break;
		}
	}
	closegraph();
	return 0;
}

void InitGame()
{
	//加载图片
	loadimage(&img[0], L"images/land.jpg", 40, 40);		//空地
	loadimage(&img[1], L"images/wall.jpg", 40, 40);		//墙
	loadimage(&img[2], L"images/BoxYellow.jpg", 40, 40);	//箱子
	loadimage(&img[3], L"images/ManDOWN1.jpg", 40, 40);	//人
	loadimage(&img[4], L"images/aim.jpg", 40, 40);		//目标
	loadimage(&img[5], L"images/BoxRed.jpg", 40, 40);	//箱子到达目的地
	loadimage(&img[6], L"images/ManDOWN2.jpg", 40, 40);	//人

	mciSendString(L"open images/Music/back.mp3 alias back", nullptr, 0, nullptr);	//文件路径采用相对路径	alias 取别名
	mciSendString(_T("play back repeat"), 0, 0, 0);		//repeat循环播放

	flag = 0;
}

void RenderGame()
{
	BeginBatchDraw();		//开始画图	防闪屏

	//循环绘制地图
	for (int y = 0; y < MAP_R; y++)		//循环每一行
	{
		for (int x = 0; x < MAP_C; x++)//循环每一列
		{
			switch (map[y][x])
			{
			case 0:putimage(x * 40, y * 40, &img[0]);	//空地
				break;
			case 1:putimage(x * 40, y * 40, &img[1]);	//墙
				break;
			case 2:putimage(x * 40, y * 40, &img[2]);	//箱子
				break;
			case 3:										//玩家
			case 7:										//玩家站在目的地上
				if(flag)
					putimage(x * 40, y * 40, &img[3]);	
				else
					putimage(x * 40, y * 40, &img[6]);	
				break;
			case 4:putimage(x * 40, y * 40, &img[4]);	//目的地
				break;
			case 6:putimage(x * 40, y * 40, &img[5]);	//箱子站在目的地上
				break;
			}
		}
	}
	EndBatchDraw();			//结束绘图
}

void UpdateGame()
{
	/*******玩家操作*******/
	if (kbhit())		//可以换成 _kbhit()  
	{
		/******找到小人******/
		for (int Row = 0; Row < MAP_R; Row++)
		{
			for (int Col = 0; Col < MAP_C; Col++)
			{
				if (map[Row][Col] == 3 || map[Row][Col] == 7)
				{
					index_Row = Row;
					index_Col = Col;
				}
			}
		}

		dir = getch();	//可以换成 _getch()    
		switch (dir)
		{
		case 'w':
		case 72:
			if (map[index_Row - 1][index_Col] == 0 || map[index_Row - 1][index_Col] == 4)
			{
				//推箱人的上面是 空地 或者 目的地  --->直接上去
				map[index_Row][index_Col] = map[index_Row][index_Col] - 3;
				map[index_Row - 1][index_Col] = map[index_Row - 1][index_Col] + 3;
			}
			if (map[index_Row - 1][index_Col] == 2 || map[index_Row - 1][index_Col] == 6)
			{
				//推箱人的上面是 箱子
				if (map[index_Row - 2][index_Col] == 0 || map[index_Row - 2][index_Col] == 4)
				{
					//箱子上面是空地或者目的地
					map[index_Row][index_Col] = map[index_Row][index_Col] - 3;
					map[index_Row - 1][index_Col] = map[index_Row - 1][index_Col] + 3 - 2;
					map[index_Row - 2][index_Col] = map[index_Row - 2][index_Col] + 2;
				}
			}
			break;
		case 's':
		case 80:
			if (map[index_Row + 1][index_Col] == 0 || map[index_Row + 1][index_Col] == 4)
			{
				//推箱人的下面是 空地 或者 目的地  --->直接上去
				map[index_Row][index_Col] = map[index_Row][index_Col] - 3;
				map[index_Row + 1][index_Col] = map[index_Row + 1][index_Col] + 3;
			}
			if (map[index_Row + 1][index_Col] == 2 || map[index_Row + 1][index_Col] == 6)
			{
				//推箱人的下面是 箱子
				if (map[index_Row + 2][index_Col] == 0 || map[index_Row + 2][index_Col] == 4)
				{
					//箱子下面是空地或者目的地
					map[index_Row][index_Col] = map[index_Row][index_Col] - 3;
					map[index_Row + 1][index_Col] = map[index_Row + 1][index_Col] + 3 - 2;
					map[index_Row + 2][index_Col] = map[index_Row + 2][index_Col] + 2;
				}
			}
			break;
		case 'a':
		case 75:
			if (map[index_Row][index_Col - 1] == 0 || map[index_Row][index_Col - 1] == 4)
			{
				//推箱人的左边是 空地 或者 目的地  --->直接上去
				map[index_Row][index_Col] = map[index_Row][index_Col] - 3;
				map[index_Row][index_Col - 1] = map[index_Row][index_Col - 1] + 3;
			}
			if (map[index_Row][index_Col - 1] == 2 || map[index_Row][index_Col - 1] == 6)
			{
				//推箱人的左边是 箱子
				if (map[index_Row][index_Col - 2] == 0 || map[index_Row][index_Col - 2] == 4)
				{
					//箱子左边是空地或者目的地
					map[index_Row][index_Col] = map[index_Row][index_Col] - 3;
					map[index_Row][index_Col - 1] = map[index_Row][index_Col - 1] + 3 - 2;
					map[index_Row][index_Col - 2] = map[index_Row][index_Col - 2] + 2;
				}
			}
			break;
		case 'd':
		case 77:
			if (map[index_Row][index_Col + 1] == 0 || map[index_Row][index_Col + 1] == 4)
			{
				//推箱人的右边是 空地 或者 目的地  --->直接上去
				map[index_Row][index_Col] = map[index_Row][index_Col] - 3;
				map[index_Row][index_Col + 1] = map[index_Row][index_Col + 1] + 3;
			}
			if (map[index_Row][index_Col + 1] == 2 || map[index_Row][index_Col + 1] == 6)
			{
				//推箱人的右边是 箱子
				if (map[index_Row][index_Col + 2] == 0 || map[index_Row][index_Col + 2] == 4)
				{
					//箱子右边是空地或者目的地
					map[index_Row][index_Col] = map[index_Row][index_Col] - 3;
					map[index_Row][index_Col + 1] = map[index_Row][index_Col + 1] + 3 - 2;
					map[index_Row][index_Col + 2] = map[index_Row][index_Col + 2] + 2;
				}
			}
			break;
		}
		PlaySound(L"../images/Music/Boxmove.WAV", nullptr, SND_FILENAME | SND_ASYNC);
		RenderGame();		//绘制游戏
		Sleep(100);
	}
}

bool isWin()
{
	for (int Row = 0; Row < MAP_R; Row++)
	{
		for (int Col = 0; Col < MAP_C; Col++)
		{
			//循环查找是否有 没在目的地 的 箱子
			if (map[Row][Col] == 2)
			{
				return false;
			}
		}
	}
	return true;
}

bool isFail()
{
	for (int Row = 0; Row < MAP_R; Row++)
	{
		for (int Col = 0; Col < MAP_C; Col++)
		{
			//寻找箱子
			if (map[Row][Col] == 2)
			{
				//判断箱子两个相连方向是否有墙
				if (map[Row - 1][Col] == 1 || map[Row + 1][Col] == 1)
				{
					if (map[Row][Col - 1] == 1 || map[Row][Col + 1] == 1)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

