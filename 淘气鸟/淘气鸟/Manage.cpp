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

//��ʼ��
void Manage::initGame()
{
	//��ʼ������
	setbkmode(TRANSPARENT);		//���ñ���͸��
	//��ʼ������
	m_bk.initBackground();
	m_bird.initBird();
	m_block.initBlock();
}

//���ݸ���
void Manage::updateGame(DWORD PassTime)
{
	//��ͣ��ʼ��Ϸ
	if (m_isStart)
	{
		m_passTime += PassTime;
		//����Ԫ��
		m_bk.updateBackground(PassTime);
		m_bird.updateBird(PassTime);
		m_block.updateBlock(PassTime);
		//��ʼ��Ϸ֮���ϰ���ͱ����ƶ�
		if (!m_bird.GetBirdIsStart())	
		{
			m_block.SetBlockIsMove(true);
			m_bk.SetBkIsMove(true);
			//�������
			if (m_passTime >= 2000)
			{
				m_passTime = 0;
				m_score++;
			}
		}
		/***************�ж���Ӯ****************/
		RECT BirdRect = m_bird.GetBirdRect();		//��ȡ�ϰ������ھ�������
		RECT* BlockRect = m_block.GetBlockRect();	//��ȡС�����ھ�������
		for (int i = 0; i < 10; i++)
		{
			//С�������ϰ���
			if (!(BirdRect.right<BlockRect[i].left || BirdRect.left>BlockRect[i].right
				|| BirdRect.top > BlockRect[i].bottom || BirdRect.bottom < BlockRect[i].top))
			{
				//��Ϸ����	��������ӽ�������
				this->initGame();
				this->startUI();
				m_isStart = false;
				m_bird.SetBirdIsStart(false);
				m_score = 0;
			}
		}
		//С��ɳ����棬��Ϸ����
		if (BirdRect.bottom >= 300 || BirdRect.top <= -44)
		{
			//��Ϸ����	��������ӽ������� ����ֱ�ӻص���ʼUI����
			this->initGame();
			this->startUI();
			m_isStart = false;
			m_bird.SetBirdIsStart(false);
			m_score = 0;
		}
	}
	//��Ҳ���
	if (kbhit())
	{
		switch (getch())
		{
		case ' ':	//�ո�
			m_bird.SetBirdIsUp(true);
			break;
		case 'p':
		case 'P':
			if (m_isStart)
				m_isStart = false;
			else
				m_isStart = true;
			break;
		case 13:	//�س�
			m_isStart = true;
			m_bird.SetBirdIsStart(true);
			break;
		default:
			break;
		}
	}
}

//������Ϸ
void Manage::drawGame()
{
	if (m_isStart)
	{
		BeginBatchDraw();
		m_bk.drawBackground();
		m_bird.drawBird();
		m_block.drawBlock();
		settextcolor(WHITE);
		settextstyle(24, 0, L"����");
		wchar_t str[20];
		wsprintf(str, L"������%d", m_score);
		outtextxy(0, 0, str);
		EndBatchDraw();
	}
}

//��ʼ����
void Manage::startUI()
{
	m_bk.drawBackground();
	settextcolor(RED);
	settextstyle(28, 0, L"����");
	outtextxy(120, 50, L"��ŭ��С��");
	settextcolor(BLACK);
	settextstyle(18, 0, L"����");
	outtextxy(135, 90, L"����˵����");
	outtextxy(120, 120, L"1�����ո����Ծ");
	outtextxy(120, 145, L"2����P����ͣ");
	outtextxy(120, 170, L"3�����س���ʼ��Ϸ");

	outtextxy(200, 200, L"VX���ںţ����ѧϰ����");
}
