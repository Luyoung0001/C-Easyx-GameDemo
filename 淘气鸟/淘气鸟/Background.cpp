#include"stafx.h"
#include "Background.h"

Background::Background()
{
}


Background::~Background()
{
}

void Background::initBackground()
{
	//初始化
	m_Dsp = 1000 / 12;	//单位是ms
	m_iMoveSpeed = 2;
	m_isMove = false;
	m_PassTime = 0;
	m_x = 0;
	m_y = 0;
	//加载图片
	loadimage(&m_bk, L"imgs/bk/background.bmp");
}

void Background::updateBackground(DWORD PassTime)
{
	if (m_isMove)
	{
		m_PassTime += PassTime;
		if (m_PassTime >= m_Dsp)
		{
			for (int i = 0; i < 2; i++)
			{
				m_x -= m_iMoveSpeed;
				if (m_x <= -WIDTH)
				{
					m_x = 0;
				}
			}
			m_PassTime -= m_Dsp;
		}
	}
}

void Background::drawBackground()
{
	//绘制两张图片模拟场景移动
	putimage(m_x, m_y, &m_bk);
	putimage(m_x + WIDTH, m_y, &m_bk);
}
