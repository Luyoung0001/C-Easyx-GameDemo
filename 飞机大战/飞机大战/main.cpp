/******************************************************************************

版权所有 (C), 2020-2021, DeRoy

******************************************************************************
文 件 名   : main.c
版 本 号   : 初稿
作    者   : DeRoy（微信公众号【编程学习基地】）
生成日期   : 2020年4月19日
最近修改   :
功能描述   : 飞机大战
函数列表   :
*
*			1.void loadimage();	//加载图片
*			2.void Game_Paint();	//游戏绘制
*			3.void Game_Init();	//游戏初始化
*			4.void Game_Updata();	//游戏更新

*			5.void BackMove();	//背景的移动
*			6.void EnemyMove();	//敌方飞机的移动
*			7.void PlaneMove();	//我方飞机的移动
*			8.void BulletMove();	//子弹的移动

*			9.void AddEnemy();	//添加敌方飞机
*			10.void FireBullet();	//发射子弹
*

修改历史   :
1.日    期   : 2020年4月19日
作    者   : DeRoy
修改内容   : 创建文件

******************************************************************************/
/*----------------------------------------------*
* 头文件包含                                     *
*----------------------------------------------*/
#include<stdio.h>
#include<easyx.h>
#include<time.h>
#include<conio.h>
/*----------------------------------------------*
* 结构体                                        *
*----------------------------------------------*/
//子弹的结构体 
struct bullet
{
	int x;
	int y;
	//子弹它是通过链表去组成
	struct bullet* pnext;
};
// 我方飞机   
struct plane  //我方飞机的结构体
{
	int x, y;			//飞机的坐标
	bool exist;			//飞机是否存活   exist==false(0)  飞机灭亡 ;  exist=ture(1) 飞机存在
	struct bullet* bt;  //子弹
};
//敌方飞机 
struct enemy
{
	int x, y; //敌方飞机的坐标
	struct enemy* pnext; //用来保存下一个飞机的地址
};

/*----------------------------------------------*
* 函数定义                                     *
*----------------------------------------------*/
void loadimage();	//加载图片
void Game_Paint();	//游戏绘制
void Game_Init();	//游戏初始化
void Game_Updata();	//游戏更新

void BackMove();	//背景的移动
void EnemyMove();	//敌方飞机的移动
void PlaneMove();	//我方飞机的移动
void BulletMove();	//子弹的移动

void AddEnemy();	//添加敌方飞机
void FireBullet();	//发射子弹

/*----------------------------------------------*
* 全局变量                                     *
*----------------------------------------------*/
COORD g_bk;			//背景的贴图位置
IMAGE g_enemy[2], g_plane[2], g_Bullet, g_back;	//背景图片
struct plane myPlane;		//定义我方飞机的结构体变量.
struct enemy* emy_Phead;	//敌方飞机链表的头节点. 
DWORD begin, end;			//定义两个时间控制移动速度
DWORD t1, t2;				//定义两个时间控制敌方飞机的产生速度


int main()
{
	initgraph(512, 768);	/* 初始化图形库 */
	Game_Init();	/* 游戏的初始化 */
	while (1)
	{
		end = GetTickCount();	/* 获取end时间 */
		t2 = GetTickCount();	/* 获取t2时间 */
		if (kbhit())			/* 判断是否有按键消息 */
		{
			PlaneMove();	/* 玩家操作飞机移动和开火 */
		}
		if (end - begin >= 50)
		{
			Game_Paint();	/* 游戏的绘制 */
			Game_Updata();	/* 游戏的跟新 */
			begin = end;
		}
		if (t2 - t1 >= 3000)
		{
			AddEnemy();		/* 添加敌方飞机 */
			AddEnemy();		/* 添加敌方飞机 */
			t1 = t2;
		}
	}
	closegraph();	/* 关闭图形库 */
	return 0;
}

/* 子弹移动 */
void BulletMove()
{
	//遍历每一个子弹 让他们向上移动
	struct bullet* pPlane = myPlane.bt;			//指向子弹的头结点
	struct bullet* pDelete;						//指向要删除的子弹
	while (pPlane->pnext != NULL)
	{
		//子弹的移动速度 都是一样的
		pPlane->pnext->y -= 10;

		//处理子弹越界
		if (pPlane->pnext->y < -50)
		{
			pDelete = pPlane->pnext;
			pPlane->pnext = pDelete->pnext;
			free(pDelete);
			pDelete = NULL;
			continue;
		}
		//处理子弹撞到飞机   子弹坐标 跟敌方飞机做比较 
		// 遍历敌方飞机 
		struct enemy *pEnemy = emy_Phead; 
		struct enemy *pDeleteEnemy;  
		//遍历敌方飞机的循环
		while (pEnemy->pnext != NULL)
		{
			if ((pPlane->pnext->x >= pEnemy->pnext->x) && pPlane->pnext->x <= (pEnemy->pnext->x + 80) && pPlane->pnext->y <= (pEnemy->pnext->y + 100))
			{
				//飞机爆炸的图片 贴上去 
				//把飞机释放掉
				pDeleteEnemy = pEnemy->pnext;
				pEnemy->pnext = pDeleteEnemy->pnext;
				free(pDeleteEnemy);
				pDeleteEnemy = NULL;

				// 把子弹释放掉 
				pDelete = pPlane->pnext;
				pPlane->pnext = pDelete->pnext;
				free(pDelete);
				pDelete = NULL;
				break;	//这个break是个坑，当子弹击中敌方某一架飞机的时候已经不需要继续遍历后面的飞机了，因为子弹已经没有了
			}
			//判断敌方飞机是不是NULL
			if (pEnemy->pnext == NULL)
			{
				break;
			}
			pEnemy = pEnemy->pnext;
		}
		//判断子弹是不是NULL
		if (pPlane->pnext == NULL)
		{
			break;  //如果子弹没了就break
		}
		pPlane = pPlane->pnext;
	}
}

/* 子弹发射 */
void FireBullet()
{
	//发射子弹其实就是创建结点.
	struct bullet *newbullet = (struct bullet *)malloc(sizeof(struct bullet));

	//给x和y 赋值
	newbullet->x = myPlane.x + 28; //子弹的x坐标		 
	newbullet->y = myPlane.y - 10;  //子弹的 y坐标

	//连接结点 
	newbullet->pnext = myPlane.bt->pnext;
	myPlane.bt->pnext = newbullet;
}

/* 我方飞机移动 */
void PlaneMove()
{
	// 获取键盘按下信息 
	char ch = getch();  //获取键盘输入
	switch (ch)		// 上 下 左  右  发射子弹
	{
	case 72:		//往上走    72 是 ↑ 键值
	case 'W':
	case 'w':
		//在里面定义变量 就要加{}
		myPlane.y -= 20;
		if (myPlane.y < 0)
		{
			myPlane.y = 0;
		}
		break;
	case 80:  //往下走
	case 'S':
	case 's':
		myPlane.y += 20;
		if (myPlane.y>668)
		{
			myPlane.y = 700;
		}
		break;
	case 75:  //左边
	case 'A':
	case 'a':
		myPlane.x -= 20;
		if (myPlane.x < 0)
		{
			myPlane.x = 0;
		}
		break;
	case 77: //右边
	case 'D':
	case 'd':
		myPlane.x += 20;
		if (myPlane.x > 437)
		{
			myPlane.x = 437;
		}
		break;
	case 32: //32是空格键
		//调用函数  发射子弹
		FireBullet();
		break;
	}
}

/* 背景移动 */
void BackMove()
{
	g_bk.Y += 2;
	if (g_bk.Y >= 768)
		g_bk.Y = 0;
}

/* 敌方飞机移动 */
void EnemyMove()
{
	struct enemy* pTmp = emy_Phead;//指向敌方飞机的头结点
	struct enemy* pDelete;

	while (pTmp->pnext != NULL)
	{
		pTmp->pnext->y += rand() % 10; //可快可慢

		//判断飞机是否越界了
		if (pTmp->pnext->y >= 800)
		{
			//把越界的飞机销毁掉
			pDelete = pTmp->pnext;
			pTmp->pnext = pDelete->pnext;
			free(pDelete);
			pDelete = NULL;
			continue;
		}
		//往后遍历
		pTmp = pTmp->pnext;
	}
}

/* 添加敌方飞机 */
void AddEnemy()
{
	//申请新结点
	struct enemy* newnode = (struct enemy*)malloc(sizeof(struct enemy));

	newnode->x = rand() % 412;	//x坐标 随机
	newnode->y = -100;			//y坐标 固定

	//头插
	newnode->pnext = emy_Phead->pnext;
	emy_Phead->pnext = newnode;
}

/* 游戏初始化 */
void Game_Init()
{
	srand((unsigned)time(NULL));						//设置随机种子
	loadimage();
	myPlane.y = 700;			
	myPlane.x = rand() % 500;   //范围 0~499 
	myPlane.exist = true;		//飞机存在为true
	//初始化子弹链表的头结点
	myPlane.bt = (struct bullet *)malloc(sizeof(struct bullet));
	myPlane.bt->pnext = NULL;

	//初始化敌方飞机的头结点
	emy_Phead = (struct enemy *)malloc(sizeof(struct enemy));
	emy_Phead->pnext = NULL;
	//初始化其他变量
	begin = GetTickCount();
	t1 = GetTickCount();
	g_bk.X = 0;
	g_bk.Y = 0;
	//开局先添加两架敌方飞机
	AddEnemy();
	AddEnemy();
}

/* 游戏初更新 */
void Game_Updata()
{
	EnemyMove();	//飞机移动
	BulletMove();	//子弹移动
	BackMove();		//背景移动
}

/* 游戏绘制 */
void Game_Paint()
{
	//开始批量绘图
	BeginBatchDraw();
	cleardevice();	//刷新屏幕
	/*========================贴背景图片========================*/

	putimage(g_bk.X, g_bk.Y, &g_back);
	putimage(g_bk.X, g_bk.Y - 768, &g_back);

	/*========================贴我方飞机========================*/
	//判断我方的飞机是否存活,如果存活就贴飞机,否者就贴游戏失败的图片.
	if (myPlane.exist)
	{
		putimage(myPlane.x, myPlane.y, &g_plane[1], SRCAND);
		putimage(myPlane.x, myPlane.y, &g_plane[0], SRCPAINT);
	}
	else
	{
		//游戏结束
		//贴 Game Over图片
	}
	/*========================贴敌方飞机========================*/
	//遍历链表去绘制
	struct enemy* pTmp = emy_Phead->pnext; //指向第一架敌方飞机
	while (pTmp)
	{
		putimage(pTmp->x, pTmp->y, &g_enemy[1], SRCAND);
		putimage(pTmp->x, pTmp->y, &g_enemy[0], SRCPAINT);
		//要往后遍历
		pTmp = pTmp->pnext;
	}

	/*========================贴我方飞机子弹========================*/
	struct bullet* pBullet = myPlane.bt->pnext;
	while (pBullet) //贴图
	{
		putimage(pBullet->x, pBullet->y, &g_Bullet, SRCPAINT);
		pBullet = pBullet->pnext;
	}
	//结束批量绘图
	EndBatchDraw();
}

/* 加载图片 */
void loadimage()
{
	//加载外部资源图片
	loadimage(&g_back, L"bg_1.bmp");
	loadimage(&g_Bullet, L"Bullet.png", 20, 20);
	loadimage(&g_enemy[0], L"smallenemy.png", 75, 75);
	loadimage(&g_enemy[1], L"smallenemy1.png", 75, 75);
	loadimage(&g_plane[0], L"plane.png", 75, 75);
	loadimage(&g_plane[1], L"plane1.png", 75, 75);
}
