
#include<stdio.h>
#include<easyx.h>
#include<conio.h>
#define MAP_ROW 4	//地图行
#define MAP_COL 4	//地图列
#define PIXEL 75	//一个格子所占像素

int MaxScore = 0;	//最高分
void init(int map[][MAP_COL],int *score);		//初始化游戏
void DrawMap(int map[][MAP_COL], int score);	//游戏渲染
void Play(int map[][MAP_COL], int*score);		//玩家操作
int Judg(int map[][MAP_COL]);					//判断输赢
void Add(int map[][MAP_COL]);					//添加数字
void WriteData(int map[][MAP_COL], int score);	//写入数据
void ReadData(int map[][MAP_COL], int* score);	//读取数据

int main()
{
	int map[MAP_ROW][MAP_COL];	//地图大小
	int score;					//总成绩
	initgraph(MAP_ROW * PIXEL + 100, MAP_COL * PIXEL + 100);	//加载窗口
	init(map, &score);			//初始化地图数据
	ReadData(map, &score);		//读取数据
	while (1)
	{
		DrawMap(map, score);	//绘制地图
		Play(map, &score);		//玩家操作
		switch (Judg(map))		//判断输赢  对结果进行匹配
		{
		case 0://赢
			MessageBox(GetHWnd(), L"你赢了", L"恭喜", MB_OK);
			if (score > MaxScore)	
				MaxScore = score;
			WriteData(map, score);
			return 0;
			break;
		case -1://输
			MessageBox(GetHWnd(), L"你输了", L"很遗憾", MB_OK);
			if (score > MaxScore)	
				MaxScore = score;
			init(map, &score);		//输了，先初始化数据
			if (MessageBox(GetHWnd(), L"是否重新开始", L"提示", MB_YESNO| MB_DEFBUTTON1) == IDNO)
			{
				WriteData(map, score);	//保存游戏
				return 0;
			}
			break;
		case 1://没有空位置
			break;
		case 2://还有空位置
			Add(map);
			break;
		default:
			break;
		}
		WriteData(map, score);	//保存游戏
	}
	getchar();
	closegraph();
	return 0;
}

void init(int map[][MAP_COL], int *score)
{
	int x, y;	//随机位置参数
	*score = 0;	//对成绩进行初始化
	memset(map, 0, sizeof(int)*MAP_ROW*MAP_COL);	//初始化地图
	//对地图进行初始化
	for (int i = 0; i < 2;)
	{
		x = rand() % 4;
		y = rand() % 4;
		if (map[x][y] == 0)
		{
			if (rand() % 9 == 0)
				map[x][y] = 4;
			else
				map[x][y] = 2;
			i++;
		}
	}
}

void DrawMap(int map[][MAP_COL],int score)
{
	wchar_t arr[20];				//临时参数
	setbkcolor(RGB(0, 0, 0));	//背景颜色
	setbkmode(TRANSPARENT);		//设置字体背景颜色透明
	setlinecolor(RGB(0, 0, 0));	//线条颜色
	setlinestyle(PS_SOLID, 3);		//线条风格
	settextcolor(RGB(0, 0, 0));	//字体颜色
	settextstyle(20, 0, L"宋体");	//字体风格 和 字体大小
	cleardevice();					//清屏
	BeginBatchDraw();				//开始绘制地图
	IMAGE imgs;
	loadimage(&imgs, L"man.jpg");
	putimage(0, 0, &imgs);
	for (int i = 0; i <= 4; ++i)
	{
		line(PIXEL * i, 0 + 50, PIXEL * i, PIXEL * 4 + 50);//竖线
		line(0, PIXEL * i + 50, PIXEL * 4, PIXEL * i + 50);//横线
	}
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			wsprintf(arr, L"%d", map[i][j]);					//将数字写入字符串
			outtextxy(j * PIXEL + 32, i * PIXEL + 50 + 30, arr);//指定位置输出字符串
		}
	}
	//分数
	settextcolor(RGB(0, 255, 0));	//字体颜色 红色
	wsprintf(arr, L"score: %d", score);
	outtextxy(0, 15, arr);			//指定位置输出字符串
	//最高分
	outtextxy(MAP_ROW * PIXEL + 10 , 5, L"最高分:");			//指定位置输出字符串
	wsprintf(arr, L"%d", MaxScore);
	outtextxy(MAP_ROW * PIXEL + 15 , 25, arr);				//指定位置输出字符串
	outtextxy(MAP_ROW * PIXEL + 10, 80, L"↑↓←→");			//指定位置输出字符串
	outtextxy(MAP_ROW * PIXEL + 10, 120, L"WSAD");			//指定位置输出字符串
	outtextxy(MAP_ROW * PIXEL + 10, 160, L"控制游戏");		//指定位置输出字符串
	outtextxy(MAP_ROW * PIXEL + 10, 280, L"QQ群：");			//指定位置输出字符串
	outtextxy(MAP_ROW * PIXEL + 5, 320, L"879098023");		//指定位置输出字符串
	outtextxy(80, 360, L"微信公众号：编程学习基地");		//指定位置输出字符串
	EndBatchDraw();		//结束绘制地图
}

void Play(int map[][MAP_COL], int * score)
{
	int len;
	switch (getch())
	{
	case 72://上
	case'W':
	case'w':
		//把0向下移动
		for (int j = 0; j < 4; j++)		//4列分别操作
		{
			len = 4;
			for (int i = 0; i < len - 1; i++)
			{
				if (map[i][j] == 0)
				{
					//下面的数字往上移一个格子
					for (int k = i; k < 3; k++)
					{
						map[k][j] = map[k + 1][j];
						map[k + 1][j] = 0;
					}
					len--;	//出现一个 0 len减一
					i--;	//返回原位置检测是否有 0
				}
			}
			for (int i = 0; i < len - 1; i++)
			{
				if (map[i][j] == 0) break;		//第一行为0 后面都是0，不进行操作
				if (map[i][j] == map[i + 1][j])	//检测相邻两个数字是否相同，相同就合并
				{
					map[i][j] *= 2;		//合并
					//合并之后后面的数字往前移动一位
					for (int k = i + 1; k < 3; k++)
					{
						map[k][j] = map[k + 1][j];
						map[k + 1][j] = 0;
					}
					//进行打分
					*score += map[i][j];
				}
			}
		}
		break;
	case 80://下
	case'S':
	case's':
		//把0向上移动
		for (int j = 0; j < 4; j++)//4列分别操作
		{
			len = 0;
			for (int i = 3; i > len; i--)
			{
				if (map[i][j] == 0)
				{
					for (int k = i; k > 0; k--)
					{
						map[k][j] = map[k - 1][j];
						map[k - 1][j] = 0;
					}
					len++;//出现一个 0     len + 1
					i++;//返回原位置检测是否有 0
				}
			}
			for (int i = 3; i > 0; i--)
			{
				if (map[i][j] == 0) break;
				if (map[i][j] == map[i - 1][j])
				{
					map[i][j] *= 2;//合并
					for (int k = i - 1; k > 0; k--)
					{
						map[k][j] = map[k - 1][j];
						map[k - 1][j] = 0;
					}
					*score += map[i][j];
				}
			}
		}
		break;
	case 75://左
	case'A':
	case'a':
		for (int i = 0; i < 4; i++)
		{
			len = 4;
			for (int j = 0; j < len - 1; j++)
			{
				if (map[i][j] == 0)
				{
					for (int k = j; k < 3; k++)
					{
						map[i][k] = map[i][k + 1];
						map[i][k + 1] = 0;
					}
					len--;
					j--;
				}
			}
			for (int j = 0; j < 3; j++)
			{
				if (map[i][j] == 0) break;
				if (map[i][j] == map[i][j + 1])
				{
					map[i][j] *= 2;
					for (int k = j + 1; k < 3; k++)
					{
						map[i][k] = map[i][k + 1];
						map[i][k + 1] = 0;
					}
					*score += map[i][j];
				}
			}
		}
		break;
	case 77://右
	case'd':
	case'D':
		for (int i = 0; i < 4; i++)
		{
			len = 0;
			for (int j = 3; j > len; j--)
			{
				if (map[i][j] == 0)
				{
					for (int k = j; k > 0; k--)
					{
						map[i][k] = map[i][k - 1];
						map[i][k - 1] = 0;
					}
					len++;
					j++;
				}
			}
			for (int j = 3; j > 0; j--)
			{
				if (map[i][j] == map[i][j - 1])
				{
					map[i][j] *= 2;
					for (int k = j - 1; k > 0; k--)
					{
						map[i][k] = map[i][k - 1];
						map[i][k - 1] = 0;
					}
					*score += map[i][j];
				}
			}
		}
		break;
	default:break;
	}
}

int Judg(int map[][MAP_COL])
{
	int num = 0;	//记录0的个数
	int flag = 0;	//如果存在一组可以合并的元素 那么flag置为1
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (map[i][j] >= 2048) return 0;//赢
			if (map[i][j] == 0) num++;		//记录0的个数
			if (j + 1 < 4 && map[i][j] == map[i][j + 1]) flag = 1;//右边是不是一样
			if (i + 1 < 4 && map[i][j] == map[i + 1][j]) flag = 1;//下方是不是一样
		}
	}
	if (num == 0 && flag == 0)  return -1;	//满了并且不能合并  输
	if (num == 0) return 1;					//满了 但是可以合并
	return 2;
}

void Add(int map[][MAP_COL])
{
	//添加数字
	int x, y;
	do
	{
		x = rand() % 4;
		y = rand() % 4;
	} while (map[x][y] != 0);	//找到数字为零的位置
	if (rand() % 9 == 0)		//设置4出现的概率为1/9
		map[x][y] = 4;
	else
		map[x][y] = 2;
}

void WriteData(int map[][MAP_COL], int score)
{
	FILE*fp = fopen("D:\\2048.dat", "wb");
	if (fp == NULL) return;
	fwrite(map, sizeof(int)*MAP_ROW*MAP_COL, 1, fp);
	fwrite(&score, sizeof(int), 1, fp);
	fwrite(&MaxScore, sizeof(int), 1, fp);
	fclose(fp);
}

void ReadData(int map[][MAP_COL], int* score)
{
	FILE*fp = fopen("D:\\2048.dat", "rb");
	if (fp==NULL) return;
	fread(map, sizeof(int)*MAP_ROW*MAP_COL, 1, fp);
	fread(score, sizeof(int), 1, fp);
	fread(&MaxScore, sizeof(int), 1, fp);
	fclose(fp);
}

