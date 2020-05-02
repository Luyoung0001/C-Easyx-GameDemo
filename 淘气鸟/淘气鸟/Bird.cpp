#include"stafx.h"
#include "Bird.h"

Bird::Bird()
{

}


Bird::~Bird()
{
}

void Bird::initBird()
{
	//初始化游戏数据
	SetRect(&m_BirdRect, 0, 0, 0, 0);
	m_Dps = 1000 / 12;
	m_PassTime = 0;
	m_isMove = false;
	m_isStart = false;
	m_isUp = false;
	m_JumpTimer = 0;
	m_index = 0;
	m_x = m_y = 0;
	//加载图片
	loadimage(&m_bird[0][0], L"imgs/bird/bird1-1.gif");
	loadimage(&m_bird[0][1], L"imgs/bird/bird1-2.gif");
	loadimage(&m_bird[1][0], L"imgs/bird/bird2-1.gif");
	loadimage(&m_bird[1][1], L"imgs/bird/bird2-2.gif");
	loadimage(&m_bird[2][0], L"imgs/bird/bird3-1.gif");
	loadimage(&m_bird[2][1], L"imgs/bird/bird3-2.gif");
	loadimage(&m_bird[3][0], L"imgs/bird/bird4-1.gif");
	loadimage(&m_bird[3][1], L"imgs/bird/bird4-2.gif");
}

void Bird::drawBird()
{
	putimage(m_x, m_y, &m_bird[m_index][0], NOTSRCERASE);
	putimage(m_x, m_y, &m_bird[m_index][1], SRCINVERT);
	setfillcolor(RED);
	fillrectangle(m_BirdRect.left, m_BirdRect.top, m_BirdRect.right, m_BirdRect.bottom);
}

void Bird::updateBird(DWORD PassTime)
{
	m_PassTime += PassTime;
	if (m_PassTime >= m_Dps)
	{
		//每隔m_Dps时间 改变小鸟状态index 改变小鸟纵坐标m_y
		m_index = (++m_index) % 3;	
		m_PassTime -= m_Dps;
		//进入场景
		if (m_isStart)
		{
			m_x += 2;
			m_y += 3;
			if (m_x >= 100)
			{
				m_isStart = false;
				m_isMove = true;
			}
		}
		//开始游戏
		if (m_isMove)
		{
			if (m_isUp)//上升
			{
				m_y -= 15;
				m_JumpTimer += PassTime;
				if (m_JumpTimer >= 10)	//跳跃时间为10ms
				{
					m_JumpTimer = 0;
					m_isUp = false;
				}
			}
			else//下降
			{
				m_y += 3;
			}
		}
	}
	//设置小鸟所在矩形
	SetRect(&m_BirdRect, m_x + 2, m_y + 2, m_x + 26, m_y + 23);
}
