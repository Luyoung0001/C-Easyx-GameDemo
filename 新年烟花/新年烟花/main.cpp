#include<stdio.h>
#include<easyx.h>
#include<conio.h>
#include<math.h>
#include<time.h>
#include <windows.h>
#pragma comment(lib,"winmm.lib") //使用音乐播放库

#define FLOWER_WIDTH 240
#define FLOWER_HIGHT 240
#define NUM 13

struct FIREWORKS
{
	int m_x, m_y;	//图片贴图的左上角坐标
	int cen_x, cen_y;	//烟花中心点坐标
	int m_r;	//烟花半径
	int max_r;	//最大半径
	int m_width, m_hight;	//图片的高度和宽度
	int m_Arr[FLOWER_HIGHT][FLOWER_WIDTH];	//储存图片的像素
	int draw;	//是否绘制
	int start;	//是否绽放
	DWORD t1, t2, dt;	//绽放的速度 GetTickCount()
}flower[NUM];	//定义烟花结构体变量

struct SHOOT
{
	int m_x, m_y;	//烟花弹的x,y坐标
	int m_hx, m_hy;	//烟花弹的最高点x,y坐标
	int shoot;		//烟花的状态 是否发射
	int height;		//发射的经历的最高处
	DWORD t1, t2, dt;	//发射速度
	IMAGE imgs[2];	//一明一暗两证图片
	int flag;		//明暗两种状态的标志
}bullet[NUM];

DWORD TimeBegin, TimeNext;	//开始时间 下一个时间点
DWORD T1;	//开始放烟花的时间
DWORD *pBuff;
MOUSEMSG msg;         //鼠标消息

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

	mciSendString(L"open 小幸运.mp3 alias NewYear", nullptr, 0, nullptr);  //文件路径采用相对路径  alias 取别名
	mciSendString(_T("play NewYear repeat"), 0, 0, 0);
	Start();
	LoadImage();
	//初始化NUM个子弹及烟花数据
	for (int i = 0; i < NUM; i++)
	{
		Init(i);
	}
	pBuff = GetImageBuffer(NULL);	//获取绘图设备的显存指针
	T1 = GetTickCount();
	TimeBegin = GetTickCount();

	BeginBatchDraw();
	while (1)
	{
		TimeNext = GetTickCount();
		CleanPixel();
		//前二十秒随机放烟花
		if (TimeNext - TimeBegin <= 20000)
		{
			Choose();
		}
		//二十六秒开始燃放心形烟花
		if (TimeNext - TimeBegin == 26000)
		{
			HeartShow();
		}
		//鼠标左键按下放烟花	Easyx里面的GetMouseMsg有点难受，一定要获取到消息才可以继续后面的代码
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
	loadimage(&NewYeay, L"背景.jpg", 1200, 800);
	putimage(0, 0, &NewYeay);
	Sleep(2000);	//暂停三秒
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
			if (py1 < 799)	//使素点变黑
			{
				pBuff[py1 * 1200 + px1] = pBuff[py1 * 1200 + px1 + 1] = BLACK;
			}
		}
	}
}

void LoadImage()
{
	//加载图片资源
	IMAGE flowers, temp;		//定义变量保存图片资源
	loadimage(&flowers, L"flower.jpg");	//加载图片资源
	for (int i = 0; i < NUM; i++)
	{
		SetWorkingImage(&flowers);//设置绘图目标为 flowers 对象
		getimage(&temp, i * 240, 0, 240, 240);
		SetWorkingImage(&temp);//设置绘图目标为 temp 对象

							   //将图片中的像素储存到烟花的结构体变量flower[NUM]中去
		for (int y = 0; y < FLOWER_HIGHT; y++)
		{
			for (int x = 0; x < FLOWER_WIDTH; x++)
			{
				flower[i].m_Arr[x][y] = getpixel(x, y);	//获取像素点
			}
		}
	}

	IMAGE bullets;
	loadimage(&bullets, L"shoot.jpg");	//将子弹全图加载进来
	for (int i = 0; i < NUM; i++)
	{
		SetWorkingImage(&bullets);
		int x = rand() % 5;
		getimage(&bullet[i].imgs[0], x * 20, 0, 20, 50);		//暗图
		getimage(&bullet[i].imgs[1], (x + 5) * 20, 0, 20, 50);	//亮图
	}
	SetWorkingImage();
}

void Init(int i)
{
	//初始化烟花子弹数据
	int r[13] = { 120, 120, 155, 123, 130, 147, 138, 138, 130, 135, 140, 132, 155 };
	int x[13] = { 120, 120, 110, 117, 110, 93, 102, 102, 110, 105, 100, 108, 110 };
	int y[13] = { 120, 120, 85, 118, 120, 103, 105, 110, 110, 120, 120, 104, 85 };

	//初始化烟花
	flower[i].cen_x = x[i];
	flower[i].cen_y = y[i];
	flower[i].max_r = r[i];
	flower[i].draw = 0;	//是否绘制烟花
	flower[i].m_width = 240;
	flower[i].m_hight = 240;
	flower[i].dt = 5;	//绽放速度
	flower[i].m_r = 0;	//当前半径为0 
	flower[i].t1 = GetTickCount();	//第一次获取时间

	//初始化烟花弹
	bullet[i].height;
	bullet[i].flag = 0;		//子弹状态为暗
	bullet[i].shoot = 0;	//不发射
	bullet[i].dt = rand() % 4 + 4;
	bullet[i].t1 = GetTickCount();	//第一次获取时间

}

void Choose()
{
	DWORD T2 = GetTickCount();	//t2
	if (T2 - T1 > 100)	//100ms产生一个新的烟花弹
	{
		int i = rand() % NUM;	//13个烟花弹中随机抽取一个

		if (bullet[i].shoot == 0 && flower[i].draw == 0)
		{
			bullet[i].m_x = rand() % 1200;
			bullet[i].m_y = rand() % 100 + 600; //600~699
			bullet[i].m_hx = bullet[i].m_x;
			bullet[i].m_hy = rand() % 300;
			bullet[i].shoot = 1;	//发射状态

			putimage(bullet[i].m_x, bullet[i].m_y, &bullet[i].imgs[bullet[i].flag], SRCINVERT);
		}
		T1 = T2;
	}
}

void Start_Shoot()
{
	//开始发射
	for (int i = 0; i < NUM; i++)
	{
		bullet[i].t2 = GetTickCount();
		//子弹向上移动
		if (bullet[i].t2 - bullet[i].t1 >= bullet[i].dt && bullet[i].shoot == 1)
		{
			putimage(bullet[i].m_x, bullet[i].m_y, &bullet[i].imgs[bullet[i].flag], SRCINVERT);
			if (bullet[i].m_y >= bullet[i].m_hy)
			{
				bullet[i].m_y -= 5;
				bullet[i].flag = (++bullet[i].flag) % 2;	//使flag在0 1之间徘徊
			}
			putimage(bullet[i].m_x, bullet[i].m_y, &bullet[i].imgs[bullet[i].flag], SRCINVERT);
			
			if (bullet[i].m_y <= bullet[i].m_hy)
			{
				putimage(bullet[i].m_x, bullet[i].m_y, &bullet[i].imgs[bullet[i].flag], SRCINVERT);
				bullet[i].shoot = 0;	//停止发射
				flower[i].m_x = bullet[i].m_x;
				flower[i].m_y = bullet[i].m_y;
				flower[i].draw = 1;		//开始绽放
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
			//显示半径逐渐增大
			if (flower[i].m_r < flower[i].max_r)
			{
				flower[i].m_r++;
				flower[i].dt = Speed[flower[i].m_r / 10];
				flower[i].start = 1;
			}


			if (flower[i].m_r >= flower[i].max_r - 1)
			{
				flower[i].start = 0;
				//烟花绘制完成之后重新初始化改烟花及子弹数据
				Init(i);
			}
			flower[i].t1 = flower[i].t2;
		}

		if (flower[i].start)
		{
			for (double a = 0; a <= 6.28; a += 0.01)				//一圈一圈的绘制
			{
				int x1 = (int)(flower[i].cen_x + flower[i].m_r*cos(a));
				int y1 = (int)(flower[i].cen_y + flower[i].m_r*sin(a));
				if (x1>0 && x1<flower[i].m_width&&y1>0 && y1<flower[i].m_hight)
				{
					int b =  flower[i].m_Arr[x1][y1] & 0xff;		//blue蓝色，所对应的像素点 
					int g = (flower[i].m_Arr[x1][y1] >> 8) & 0xff;	//green绿色，所对应的像素点 
					int r = (flower[i].m_Arr[x1][y1] >> 16);		//red红色    对应的大小 

					int xx = (int)(flower[i].m_x + flower[i].m_r*cos(a));
					int yy = (int)(flower[i].m_y + flower[i].m_r*sin(a));

					if (r>0x20 && r>0x20 && g>0x20 && xx>0 && xx<1200 && yy>0 && yy<800)	//去掉暗色点
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
			bullet[i].m_x = x[i] * 10 - 90;		//发射点的横坐标
			bullet[i].m_y = (y[i] + 60) * 10;	//发射点的纵坐标
			bullet[i].m_hx = bullet[i].m_x;		//最高点横坐标
			bullet[i].m_hy = y[i] * 10;			//最高点纵坐标
			bullet[i].height = bullet[i].m_y - bullet[i].m_hy;//高度一样

			bullet[i].shoot = 1;
			putimage(bullet[i].m_x, bullet[i].m_y, &bullet[i].imgs[bullet[i].flag], SRCINVERT);
		}
	}
}

void MouseMsg()
{
	static int index = 0;
	msg = GetMouseMsg();  //获取鼠标消息
	switch (msg.uMsg)    //对鼠标消息进行匹配
	{
	case WM_LBUTTONDOWN:  //当鼠标消息是左键按下时
	{
		//int i = rand() % NUM;	//13个烟花弹中随机抽取一个
		if (bullet[index].shoot == 0 && flower[index].draw == 0)
		{
			bullet[index].m_x = msg.x;
			bullet[index].m_y = rand() % 100 + 600; //600~699
			bullet[index].m_hx = bullet[index].m_x;
			bullet[index].m_hy = rand() % 300;
			bullet[index].shoot = 1;	//发射状态
			putimage(bullet[index].m_x, bullet[index].m_y, &bullet[index].imgs[bullet[index].flag], SRCINVERT);
			index = index++ % 13;
		}

	}
		break;
	default:
		break;
	}
}
