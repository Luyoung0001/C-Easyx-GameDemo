/******************************************************************************

                  版权所有 (C), 2001-2011, DeRoy

 ******************************************************************************
  文 件 名   : main.cpp
  版 本 号   : 初稿
  作    者   : DeRoy
  生成日期   : 2019年10月29日
  最近修改   : 2019年11月21日
  功能描述   : 拼图游戏
  函数列表   :
*
*       1.                DrawMap	绘制地图
*       2.                GameInit	初始化游戏数据
*       3.                init		加载资源
*       4.                inverseNumber	逆序数
*       5.                Judg		判断输赢
*       6.                main		主函数
*       7.                play		玩家操作
*       8.                start		开始界面
*
  修改历史   :
  1.日    期   : 2019年11月21日
    作    者   : DeRoy
    修改内容   : 新增奇偶排列判断，地图数据打乱算法升级
				新增鼠标中键更换背景图片
******************************************************************************/
#include<stdio.h>
#include<easyx.h>	//图形库
#include<time.h>	//随机数
#include<conio.h>	//getch()
//使用音乐播放库
#include<Windows.h>
#pragma  comment (lib,"Winmm.lib") 

#define N 4				//地图是4×4格式

IMAGE img[4], imgs[16];	//img[4] 4张原图，四个关卡;	imgs分割图片
int aim_c, aim_r;		//假定空白处为目标  参数为目标所在位置的	行 列
int map[4][4] = { 0 };	//地图初始化为0
int NUM = 0;			//关卡

//游戏规则，开始界面设计
void start();

//加载资源
void init();

//游戏数据初始化
void GameInit();

//游戏渲染
void DrawMap();

//玩家操作
void play();

//判断输赢
void Judg();

//返回 map 数据的逆序数
int inverseNumber();

int main()
{
	initgraph(N * 100, N * 100);	//初始化窗口大小为400*400
	start();						//开始界面
	init();							//加载资源
	GameInit();						//游戏初始化
	DrawMap();						//游戏渲染
	while (1)	
	{
		play();						//玩家操作
		Judg();						//判断输赢
	}
	closegraph();					//关闭窗口
	return 0;
}

void start()
{
	setbkcolor(WHITE);		//设置窗口背景色为白色
	cleardevice();			//刷新屏幕
	
	setbkmode(TRANSPARENT);		//设置字体背景色为透明
	settextcolor(BLACK);		//设置字体颜色为黑色
	/*****************游戏规则*****************/
	outtextxy(170, 20, L"游戏规则：");
	outtextxy(50, 50, L"1.鼠标左键点击空白图处周围图片交换位置");
	outtextxy(50, 100, L"2.鼠标右键任意处按下显示参照图片");
	outtextxy(50, 150, L"3.鼠标中建更换背景图片");
	outtextxy(50, 200, L"4.按回车键开始游戏");

	outtextxy(20, 250, L"相关内容：");
	outtextxy(50, 300, L"关注微信公众号: 编程学习基地 获取源码");
	outtextxy(50, 350, L"编程学习交流群:  879098023");
	outtextxy(50, 400, L"背景音乐:  爱河");
	getch();
}

void init()
{
	//加载资源图片    4张图片4个关卡
	loadimage(&img[0], L"images/1.jpg", 400, 400);
	loadimage(&img[1], L"images/2.jpg", 400, 400);
	loadimage(&img[2], L"images/3.jpg", 400, 400);
	loadimage(&img[3], L"images/4.jpg", 400, 400);

	//设置最后一张图片为空白图片，作为目标图片
	loadimage(&imgs[15], L"images/15.jpg", 100, 100);

	/********播放音乐********/
	//文件路径采用相对路径	alias 取别名
	mciSendString(L"open images/music/爱河.mp3 alias back", nullptr, 0, nullptr);	
	mciSendString(_T("play back repeat"), 0, 0, 0);		//repeat循环播放

	srand((unsigned)time(NULL));						//设置随机种子
}

void GameInit()
{
	//把拼图图片贴上去
	putimage(0, 0, &img[NUM]);

	//设置绘图目标为 img 对象		对拼图图片进行切割
	SetWorkingImage(&img[NUM]);
	for (int y = 0, n = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			if (n == 15)	break;
			//逐个获取100*100像素图片 存储在imgs里面
			getimage(&imgs[n++], x * 100, y * 100, (x + 1) * 100, (y + 1) * 100);
		}
	}
	//设置绘图目标为绘图窗口
	SetWorkingImage();
	map[N - 1][N - 1] = 15;
	do
	{
		int index;      //a数组的下标
		int size = 15;  //a数组的元素个数
		int a[15] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				//右下角空白可交换图片不动
				if (i == 3 && j == 3) continue;
				//从15个数字中随机取一个
				index = rand() % size;
				//将随机抽取d额数字逐个放入地图中
				map[i][j] = a[index];
				//将抽取的位置后面d额数字往前挪一位
				for (int k = index; k < size; k++)
				{
					a[k] = a[k + 1];
				}
				//控制数组长度 抽取了一张图片长度减一
				size--;
			}
		}
	} while (inverseNumber() % 2);	//当 map里面的数据 不是偶排列时执行上面循环
}

void DrawMap()
{
	BeginBatchDraw();	//开始渲染图片
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			putimage(x * 100, y * 100, &imgs[map[y][x]]);
		}
	}
	EndBatchDraw();		//结束渲染图片
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
		col = msg.x / 100;
		if (msg.x == 400)
			col = 3;
		//获取鼠标按下所在行
		row = msg.y / 100;
		if (msg.y == 400)
			row = 3;
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
			row == aim_r - 1 && col == aim_c){
			//鼠标点击图片和空白目标图片交换
			map[aim_r][aim_c] = map[row][col];
			map[row][col] = 15;
		}
		DrawMap();
		break;
	case WM_RBUTTONDOWN:	//当鼠标消息是右键按下时
		putimage(0, 0, &img[NUM]);	//将关卡图片贴到窗口上
		break;
	case WM_RBUTTONUP:		//当鼠标消息是右键抬起时
		DrawMap();			//重新绘制地图
		break;
	case WM_MBUTTONDOWN:
	{
		//打开文件夹调用的是windows提供的API，代码是文档里面查找的
		OPENFILENAME ofn;       // common dialog box structure
		wchar_t szFile[260];       // buffer for file name
		//HANDLE hf;              // file handle
								// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrFile = szFile;
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = L"All\0*.*\0jpg\0*.jpg\0";
		//ofn.nFilterIndex = 1;
		//ofn.lpstrFileTitle = NULL;
		//ofn.nMaxFileTitle = 0;
		//ofn.lpstrInitialDir = NULL;
		//ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn) == TRUE)
		{
			loadimage(&img[NUM], szFile, 400, 400);
			GameInit();	//游戏初始化
			DrawMap();	//渲染地图
		}
	}
		break;
	}
}

void Judg()
{
	//判断当每张图片是否在对应位置
	if (inverseNumber() == 0)
	{
		//挑战成功之后将全图贴上
		putimage(0, 0, &img[NUM++]);
		//四个关卡都胜利之后退出程序
		if (NUM == 4){
			MessageBox(GetHWnd(), L"挑战成功", L"Vectory", MB_OK);
			exit(0);
			return;
		}
		//没过一个关卡判断是否进入下一个关卡
		if(MessageBox(GetHWnd(), L"是否进入下一关", L"Vectory", MB_YESNO) == IDYES){
			//重新开始游戏
			GameInit();	//游戏初始化
			DrawMap();	//渲染地图
		}
		//退出游戏
		else  exit(0);
	}
}

/*****************************************************************************
 函 数 名  : inverseNumber
 功能描述  : 奇偶排列判断函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年11月21日
    作    者   : DeRoy
    修改内容   : 新生成函数
*****************************************************************************/
int inverseNumber()
{
	int sum = 0, k = 0;		//sum为逆序数
	int arr[ N * N ];		
	//将map里面的数据导入到一维数组arr里面去
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			arr[k++] = map[i][j];
		}
	}
	//求逆序数
	for (int i = 1; i < N * N; i++)		//从第二个数起有逆序数
	{
		for (int j = 0; j < i; j++)		//循环遍历 第i个数 前面的数
		{
			if (arr[i] < arr[j])
			{
				sum++;
			}
		}
	}
	return sum;	//返回逆序数
}



