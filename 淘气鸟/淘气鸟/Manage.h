#pragma once

class Manage
{
	Bird m_bird;		//С����
	Background m_bk;	//������
	Block m_block;		//�ϰ���
	bool m_isStart;		//�Ƿ�ʼ��Ϸ
	DWORD m_passTime;	//������ʱ�� �����������
	int m_score;		//����
public:
	Manage();
	~Manage();
	void initGame();					//��ʼ��
	void updateGame(DWORD PassTime);	//���ݸ���
	void drawGame();					//������Ϸ
	void startUI();						//��ʼ����
};

