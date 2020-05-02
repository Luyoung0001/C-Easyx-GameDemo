#include"stafx.h"
#include"Manage.h"

Manage::Manage()
{
	m_isStart = false;
	m_score = 0;
	m_passTime = 0;
}


Manage::~Manage()
{
}

//初始化
void Manage::initGame()
{
	//初始化其他
	setbkmode(TRANSPARENT);		//设置背景透明
	//初始化对象
	m_bk.initBackground();
	m_bird.initBird();
	m_block.initBlock();
}

//数据更新
void Manage::updateGame(DWORD PassTime)
{
	//暂停或开始游戏
	if (m_isStart)
	{
		m_passTime += PassTime;
		//更新元素
		m_bk.updateBackground(PassTime);
		m_bird.updateBird(PassTime);
		m_block.updateBlock(PassTime);
		//开始游戏之后障碍物和背景移动
		if (!m_bird.GetBirdIsStart())	
		{
			m_block.SetBlockIsMove(true);
			m_bk.SetBkIsMove(true);
			//计算分数
			if (m_passTime >= 2000)
			{
				m_passTime = 0;
				m_score++;
			}
		}
		/***************判断输赢****************/
		RECT BirdRect = m_bird.GetBirdRect();		//获取障碍物所在矩形区域
		RECT* BlockRect = m_block.GetBlockRect();	//获取小鸟所在矩形区域
		for (int i = 0; i < 10; i++)
		{
			//小鸟碰到障碍物
			if (!(BirdRect.right<BlockRect[i].left || BirdRect.left>BlockRect[i].right
				|| BirdRect.top > BlockRect[i].bottom || BirdRect.bottom < BlockRect[i].top))
			{
				//游戏结束	可自行添加结束界面
				this->initGame();
				this->startUI();
				m_isStart = false;
				m_bird.SetBirdIsStart(false);
				m_score = 0;
			}
		}
		//小鸟飞出界面，游戏结束
		if (BirdRect.bottom >= 300 || BirdRect.top <= -44)
		{
			//游戏结束	可自行添加结束界面 这里直接回到开始UI界面
			this->initGame();
			this->startUI();
			m_isStart = false;
			m_bird.SetBirdIsStart(false);
			m_score = 0;
		}
	}
	//玩家操作
	if (kbhit())
	{
		switch (getch())
		{
		case ' ':	//空格
			m_bird.SetBirdIsUp(true);
			break;
		case 'p':
		case 'P':
			if (m_isStart)
				m_isStart = false;
			else
				m_isStart = true;
			break;
		case 13:	//回车
			m_isStart = true;
			m_bird.SetBirdIsStart(true);
			break;
		default:
			break;
		}
	}
}

//绘制游戏
void Manage::drawGame()
{
	if (m_isStart)
	{
		BeginBatchDraw();
		m_bk.drawBackground();
		m_bird.drawBird();
		m_block.drawBlock();
		settextcolor(WHITE);
		settextstyle(24, 0, L"宋体");
		wchar_t str[20];
		wsprintf(str, L"分数：%d", m_score);
		outtextxy(0, 0, str);
		EndBatchDraw();
	}
}

//开始界面
void Manage::startUI()
{
	m_bk.drawBackground();
	settextcolor(RED);
	settextstyle(28, 0, L"宋体");
	outtextxy(120, 50, L"愤怒的小鸟");
	settextcolor(BLACK);
	settextstyle(18, 0, L"宋体");
	outtextxy(135, 90, L"操作说明：");
	outtextxy(120, 120, L"1、按空格键跳跃");
	outtextxy(120, 145, L"2、按P键暂停");
	outtextxy(120, 170, L"3、按回车开始游戏");

	outtextxy(200, 200, L"VX公众号：编程学习基地");
}
