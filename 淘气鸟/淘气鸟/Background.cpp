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
	//��ʼ��
	m_Dsp = 1000 / 12;	//��λ��ms
	m_iMoveSpeed = 2;
	m_isMove = false;
	m_PassTime = 0;
	m_x = 0;
	m_y = 0;
	//����ͼƬ
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
	//��������ͼƬģ�ⳡ���ƶ�
	putimage(m_x, m_y, &m_bk);
	putimage(m_x + WIDTH, m_y, &m_bk);
}
