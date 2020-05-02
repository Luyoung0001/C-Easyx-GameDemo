#include"stafx.h"
#include "Block.h"

Block::Block()
{
}


Block::~Block()
{
}

void Block::initBlock()
{
	//初始化游戏数据
	m_Dps = 1000 / 12;
	m_PassTime = 0;
	m_iMoveSpeed = 2;
	m_isMove = false;
	ZeroMemory(m_BlockRect, sizeof(m_BlockRect));
	//加载图片
	loadimage(&m_block, L"imgs/stone/block.bmp");
	POINT pt[10] = { { 200, -20 },{ 200, 200 },{ 350, -60 },{ 350, 160 },{ 500, -40 },{ 500, 180 },{ 650, 0 },{ 650, 220 },{ 800, -60 },{ 800, 160 }};
	for (int i = 0; i < 10; i++)
	{
		SetRect(&m_BlockRect[i], pt[i].x, pt[i].y, pt[i].x + 32/*这是墙图片的宽度*/, pt[i].y + 153);
	}
}

void Block::updateBlock(DWORD PassTime)
{
	if (m_isMove)
	{
		m_PassTime += PassTime;
		if (m_PassTime >= m_Dps)
		{
			for (int i = 0; i < 10; i++)
			{
				m_BlockRect[i].left -= 2;
				m_BlockRect[i].right -= 2;
			}
			m_PassTime -= m_Dps;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		if (m_BlockRect[i].left < -50)
		{
			//m_BlockRect[i].left += 750;
			m_BlockRect[i].left = m_BlockRect[i].left + rand() % 75 + 675;
			m_BlockRect[i].right = m_BlockRect[i].left + 32;
		}
	}
}

void Block::drawBlock()
{
	for (int i = 0; i < 10; i++)
	{
		putimage(m_BlockRect[i].left, m_BlockRect[i].top, &m_block);
		setfillcolor(RED);
		fillrectangle(m_BlockRect[i].left, m_BlockRect[i].top, m_BlockRect[i].right, m_BlockRect[i].bottom);
	}
}
